#include "AssimpHelper.hpp"
#include "SkeletonMesh.h"
#include "Subset.h"
#include <optional>

USING(ENGINE)

SkeletonMesh::SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: StaticMesh(_pDevice)
{
	
}

SkeletonMesh::SkeletonMesh(const SkeletonMesh& _rOther)
	: StaticMesh(_rOther) ,
	bHasAnimation{ _rOther.bHasAnimation},
	RootNodeName{ _rOther  .RootNodeName },
	AnimInfoTable{ _rOther.AnimInfoTable },
	BoneSkinningMatries{ _rOther.BoneSkinningMatries } ,
	VTFPitch{ _rOther.VTFPitch } 
{

}

void SkeletonMesh::AnimationEditor()&
{
	if (AnimInfoTable)
	{
		for (auto& AnimInfoIter : *AnimInfoTable)
		{
			auto& AnimInfo = AnimInfoIter.second;
			if (ImGui::TreeNode(AnimInfo.Name.c_str()))
			{
				ImGui::BulletText("Name : %s", AnimInfo.Name.c_str());
				ImGui::BulletText("Duration : %2.3f", AnimInfo.Duration);
				ImGui::BulletText("TickPerSecond : %2.3f", AnimInfo.TickPerSecond);
				ImGui::SliderFloat("Acceleration", &AnimInfo.Acceleration, 0.1f, 10.f);
				ImGui::SliderFloat("TransitionTime", &AnimInfo.TransitionTime, 0.1f, 10.f);

				if (AnimInfo.Name == AnimName && !bAnimationEnd)
				{
					if (ImGui::Button("Stop"))
					{
						StopAnimation();
					}
					ImGui::SameLine();
					if (ImGui::Button("Continue"))
					{
						ContinueAnimation();
					}
					float PlayTime = PlayingTime();
					if (ImGui::SliderFloat("Playing Time", &PlayTime, 0.0, 1.0f))
					{
						SetPlayingTime(PlayTime);
					}
				}
				else
				{
					if ((ImGui::Button("Play")))
					{
						PlayAnimation(AnimInfo.Name,
							true, AnimInfo.Acceleration, AnimInfo.TransitionTime, {});
					}
				}
				ENGINE::AnimNotify _Notify{};

				ImGui::TreePop();
			}
			
		}
	}
}

void SkeletonMesh::NodeEditor()
{
	if (auto RootNodeIter = Nodes.find(RootNodeName);
		RootNodeIter != std::end(Nodes))
	{
		auto SpNode = RootNodeIter->second;
		if (SpNode)
		{
			SpNode->Editor();
		}
	}
}

void SkeletonMesh::AnimationUpdateImplementation()&
{
	AnimationNotify();
	std::optional<AnimationBlendInfo> IsAnimationBlend = std::nullopt;

	if (TransitionRemainTime > 0.0)
	{
		if (PrevAnimMotionTime > (*AnimInfoTable)[PrevAnimName].Duration)
		{
			PrevAnimMotionTime = (*AnimInfoTable)[PrevAnimName].Duration;
		}

		AnimationBlendInfo _PrevAnimBlend;
		_PrevAnimBlend.PrevAnimationName = PrevAnimName;
		_PrevAnimBlend.AnimationTime = PrevAnimMotionTime;
		_PrevAnimBlend.PrevAnimationWeight =
			TransitionRemainTime / TransitionDuration;
		IsAnimationBlend = _PrevAnimBlend;
	}


	auto* const Root = GetRootNode();
	Root->NodeUpdate(FMath::Identity(), CurrentAnimMotionTime,
		AnimName, IsAnimationBlend);


	if (CurrentAnimMotionTime > CurPlayAnimInfo.Duration)
	{
		AnimationEnd();
	}

	VTFUpdate();
}

void SkeletonMesh::Save()&
{

}

void SkeletonMesh::Load()&
{

}

void SkeletonMesh::Free()
{
	StaticMesh::Free();

	if (BoneAnimMatrixInfo)
	{
		BoneAnimMatrixInfo->Release();
	}
	if (PrevBoneAnimMatrixInfo)
	{
		PrevBoneAnimMatrixInfo->Release();
	}
};

SkeletonMesh* SkeletonMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, 
								   const std::filesystem::path _Path)
{
	SkeletonMesh* pInstance = new SkeletonMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_Path)))
	{
		pInstance->Free();
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

Resource* SkeletonMesh::Clone()
{
	SkeletonMesh* pClone = new SkeletonMesh(*this);

	if (pClone->bClone)
	{
		pClone->InitTextureForVertexTextureFetch();
		pClone->Nodes.clear();
		pClone->MakeHierarchyForclones(nullptr, GetRootNode());
	}

	return pClone;
}

void SkeletonMesh::Editor()
{
	if (bEdit)
	{
		AnimationEditor();
		NodeEditor();
	}
	Mesh::Editor();
}

std::string SkeletonMesh::GetName()
{
	return "SkeletonMesh";
}

void SkeletonMesh::BindVTF(ID3DXEffect* Fx)&
{
	Fx->SetTexture("VTF", BoneAnimMatrixInfo);
	Fx->SetInt("VTFPitch", VTFPitch);
}

HRESULT SkeletonMesh::LoadMeshFromFile(const std::filesystem::path _Path)&
{
	//Assimp Importer 생성.
	auto AiImporter = Assimp::Importer{};
	//FBX파일을 읽어서 Scene 생성.
	const aiScene* const AiScene = AiImporter.ReadFile(
		_Path.string(),
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_FindInstances |
		aiProcess_GenSmoothNormals |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes |
		aiProcess_SplitLargeMeshes
	);

	return LoadSkeletonMeshImplementation(AiScene, _Path);
}


void SkeletonMesh::EnablePrevVTF()&
{
	if (!PrevBoneAnimMatrixInfo)
	{
		m_pDevice->CreateTexture
		(VTFPitch, VTFPitch, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED,
			&PrevBoneAnimMatrixInfo, nullptr);
		PrevBoneSkinningMatries = BoneSkinningMatries;
	}
}

void SkeletonMesh::DisablePrevVTF()&
{
	if (PrevBoneAnimMatrixInfo)
	{
		PrevBoneAnimMatrixInfo->Release();
		PrevBoneAnimMatrixInfo = nullptr;
		PrevBoneSkinningMatries.clear();
		PrevBoneSkinningMatries.shrink_to_fit();
	}
}

void SkeletonMesh::Update(const float DeltaTime)&
{
	if (bAnimationEnd || bAnimStop)return;

	CurrentAnimMotionTime += (DeltaTime * Acceleration);
	PrevAnimMotionTime += (DeltaTime * PrevAnimAcceleration);
	TransitionRemainTime -= DeltaTime;
	AnimationUpdateImplementation();
}

void SkeletonMesh::VTFUpdate()&
{
	if (PrevBoneAnimMatrixInfo)
	{
		std::swap(BoneAnimMatrixInfo, PrevBoneAnimMatrixInfo);
	}

	for (const auto& [NodeName, CurNode] : Nodes)
	{
		const int32 CurNodeIndex = CurNode->Index;

		if (CurNodeIndex != -1)
		{
			BoneSkinningMatries[CurNodeIndex] = CurNode->Final;
		}
	}

	D3DLOCKED_RECT LockRect{ 0u, };
	if (FAILED(BoneAnimMatrixInfo->LockRect(0u, &LockRect, nullptr, 0)))
	{
		assert(NULL);
	}

	std::memcpy(LockRect.pBits, BoneSkinningMatries.data(),
		BoneSkinningMatries.size() * sizeof(Matrix));
	BoneAnimMatrixInfo->UnlockRect(0u);
}

Node* SkeletonMesh::GetRootNode()&
{
	return GetNode(RootNodeName);
}

Node* SkeletonMesh::GetNode(const std::string& NodeName)&
{
	auto iter = Nodes.find(NodeName);
	if (iter != std::end(Nodes))
	{
		if (iter->second)
		{
			return iter->second.get();
		}
	}

	return nullptr;
}

void SkeletonMesh::PlayAnimation(
	const std::string& InitAnimName, 
	const bool  bLoop,
	const float AnimAcceleration,
	const float TransitionTime,
	const AnimNotify& _Notify)
{
	auto iter = AnimInfoTable->find(InitAnimName);
	if (iter == std::end(*AnimInfoTable))
	{
		return;
	}
	bAnimStop = false;
	bAnimationEnd = false;
	this->bLoop=bLoop;
	PrevAnimMotionTime = CurrentAnimMotionTime;
	CurrentAnimMotionTime = 0.0;
	PrevAnimName = AnimName;
	AnimName = InitAnimName;
	TransitionDuration = TransitionRemainTime = TransitionTime;
	PrevAnimAcceleration = Acceleration;
	Acceleration = AnimAcceleration;
	CurAnimNotify = _Notify;
	PrevPlayAnimInfo = CurPlayAnimInfo;
	CurPlayAnimInfo=iter->second;
}

void SkeletonMesh::ContinueAnimation()&
{
	if (bAnimationEnd)
		return;

	bAnimStop = false;
}

void SkeletonMesh::StopAnimation()
{
	if (bAnimationEnd)
		return;

	bAnimStop = true;
}


void SkeletonMesh::AnimationEnd()&
{
	if (bLoop)
	{
		PlayAnimation(AnimName,  bLoop,
			Acceleration, TransitionDuration, CurAnimNotify);
		bAnimationEnd = false;
	}
	else
	{
		bAnimationEnd = true;
	}
}

float SkeletonMesh::PlayingTime()
{
	if(bAnimationEnd)
		return 0.0f;
	
	return CurrentAnimMotionTime / CurPlayAnimInfo.Duration;
}

void SkeletonMesh::SetPlayingTime(float NewTime)
{
	if (bAnimationEnd)return;
	NewTime = std::clamp(NewTime, 0.0f, 1.0f); 
	const float AnimDelta = NewTime - PlayingTime();
	const float SetTime = NewTime * CurPlayAnimInfo.Duration;
	CurrentAnimMotionTime = SetTime;
	PrevAnimMotionTime += AnimDelta * PrevPlayAnimInfo.Acceleration;
	TransitionRemainTime -= AnimDelta;
	AnimationUpdateImplementation();
}

std::optional<AnimationInformation> 
	SkeletonMesh::GetAnimInfo(const std::string& AnimName) const&
{
	if (AnimInfoTable)
	{
		auto iter = AnimInfoTable->find(AnimName);
		if (iter != std::end( * AnimInfoTable))
		{
			return iter->second;
		}
	}

	return std::nullopt;
}

static aiNode* FindBone(aiNode* AiNode ,
				const std::set<std::string>& BoneNameSet)
{
	if (BoneNameSet.contains(AiNode->mName.C_Str()))
	{
		return AiNode;
	}

	for (uint32 i = 0; i < AiNode->mNumChildren; ++i)
	{
		if (auto IsFindBone = FindBone(AiNode->mChildren[i], BoneNameSet);
			IsFindBone)
		{
			return IsFindBone;
		}
	}

	return nullptr;
};


HRESULT SkeletonMesh::LoadSkeletonMeshImplementation(
	const aiScene* AiScene,
	const std::filesystem::path _Path)
{
	//Subset을 보관하는 vector 메모리 공간 확보.
	m_vecSubset.resize(AiScene->mNumMeshes);
	//FBX의 Scene을 구성하는 Mesh(Subset)로 부터 데이터 구성.
	std::unordered_map<std::string,
		std::pair<uint32, Matrix>> BoneTableParserInfo{};

	for (uint32 MeshIdx = 0u; MeshIdx < AiScene->mNumMeshes; ++MeshIdx)
	{
		const auto* const AiMesh = AiScene->mMeshes[MeshIdx];

		std::shared_ptr<Subset> _CurrentSubset;
		_CurrentSubset.reset(Subset::Create(m_pDevice), Deleter<Object>());

		VERTEXBUFFERDESC tVBDesc;
		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
		LPDIRECT3DINDEXBUFFER9	pIB = nullptr;

		if (FAILED(AssimpHelper::LoadMesh(AiMesh, m_pDevice,
			&tVBDesc, &pVB, &pIB , &BoneTableParserInfo)))
			return E_FAIL;

		MATERIAL tMaterial;

		if (FAILED(AssimpHelper::LoadMaterial(
			AiScene,
			AiMesh,
			{
				aiTextureType::aiTextureType_AMBIENT,
				aiTextureType::aiTextureType_AMBIENT_OCCLUSION,
				aiTextureType::aiTextureType_BASE_COLOR,
				aiTextureType::aiTextureType_DIFFUSE,
				aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS,
				aiTextureType::aiTextureType_DISPLACEMENT,
				aiTextureType::aiTextureType_EMISSION_COLOR,
				aiTextureType::aiTextureType_EMISSIVE,
				aiTextureType::aiTextureType_HEIGHT,
				aiTextureType::aiTextureType_LIGHTMAP,
				aiTextureType::aiTextureType_METALNESS,
				aiTextureType::aiTextureType_NORMALS,
				aiTextureType::aiTextureType_NORMAL_CAMERA,
				aiTextureType::aiTextureType_OPACITY,
				aiTextureType::aiTextureType_REFLECTION,
				aiTextureType::aiTextureType_SHININESS,
				aiTextureType::aiTextureType_SPECULAR,
				aiTextureType::aiTextureType_UNKNOWN
			},
			_Path.parent_path(),
			m_pDevice,
			&tMaterial
		)))
		{
			SafeRelease(pVB);
			SafeRelease(pIB);
			return E_FAIL;
		}

		SafeAddRef(tVBDesc.pVertexDecl);

		_CurrentSubset->Initialize(pVB, pIB, tVBDesc, tMaterial);
		m_vecSubset[MeshIdx] = _CurrentSubset;
	};

	std::set<std::string> BoneNameSet{};
	for (const auto&  [BoneName,Info] :BoneTableParserInfo)
	{
		BoneNameSet.insert(BoneName);
	}

	BoneSkinningMatries.resize(BoneTableParserInfo.size());

	RootNodeName = AiScene->mRootNode->mName.C_Str();
	MakeHierarchy(nullptr,AiScene->mRootNode, BoneTableParserInfo);

	bHasAnimation = AiScene->HasAnimations(); 
	if (bHasAnimation)
	{
		AnimInfoTable = std::make_shared
			<std::map<std::string, AnimationInformation>>();

		for (uint32 AnimIdx = 0u; AnimIdx < AiScene->mNumAnimations; ++AnimIdx)
		{
			aiAnimation* _AiAnimation = AiScene->mAnimations[AnimIdx];

			AnimationInformation CurAnimInfo{};
			CurAnimInfo.TickPerSecond = _AiAnimation->mTicksPerSecond;
			CurAnimInfo.Acceleration = 1.0 *  CurAnimInfo.TickPerSecond;
			CurAnimInfo.Name = _AiAnimation->mName.C_Str();
			CurAnimInfo.Duration = _AiAnimation->mDuration;

			for (uint32 ChannelIdx = 0u;
				ChannelIdx < _AiAnimation->mNumChannels; ++ChannelIdx)
			{
				const auto& CurChannelkey = _AiAnimation->mChannels[ChannelIdx];
				const std::string NodeName =CurChannelkey->mNodeName.C_Str();
				auto iter = Nodes.find(NodeName);
				auto SpNode = iter->second;
				auto& CurAnimTrack = SpNode->_AnimationTrack[CurAnimInfo.Name];
				auto& RefScaleTrack = CurAnimTrack.ScaleTimeLine;
				auto& RefQuatTrack= CurAnimTrack.QuatTimeLine;
				auto& RefPosTrack = CurAnimTrack.PosTimeLine;
				
				for (uint32 ScaleKeyIdx = 0u;
					ScaleKeyIdx < CurChannelkey->mNumScalingKeys;
					++ScaleKeyIdx)
				{
					const auto& CurScaleKey = CurChannelkey->mScalingKeys[ScaleKeyIdx];

					RefScaleTrack.insert({ CurScaleKey .mTime,
						AssimpHelper::ConvertVec3(CurScaleKey .mValue)});
				}

				for (uint32 QuatKeyIdx = 0u;
					QuatKeyIdx < CurChannelkey->mNumRotationKeys;
					++QuatKeyIdx)
				{
					const auto& CurQuatKey = CurChannelkey->mRotationKeys[QuatKeyIdx];
					auto Quat = AssimpHelper::ConvertQuat(CurQuatKey.mValue);
					D3DXQuaternionNormalize(&Quat , &Quat);

					RefQuatTrack.insert({
						CurQuatKey.mTime,
						Quat
						});
				}

				for (uint32 PosKeyIdx = 0u;
					PosKeyIdx < CurChannelkey->mNumPositionKeys;
					++PosKeyIdx)
				{
					const auto& CurPosKey = CurChannelkey->mPositionKeys[PosKeyIdx];
					
					RefPosTrack.insert({
						CurPosKey.mTime,
						AssimpHelper::ConvertVec3( CurPosKey.mValue )
						});
				}
			}
			
			AnimInfoTable->insert({CurAnimInfo.Name , CurAnimInfo});
		}
	}

	MakeVertexLcationsFromSubset();

	return S_OK;
}

static bool IsBone(
	const std::set<std::string>& BoneNameSet,
	const aiNode* const AiNode)
{
	if (AiNode)
	{
		return BoneNameSet.contains(AiNode->mName.C_Str());
	}
	
	return false;
};

 Node* SkeletonMesh::MakeHierarchy(Node* BoneParent,
	const aiNode* const AiNode,
	const std::unordered_map<std::string,
	std::pair<uint32, Matrix>>& BoneTableParserInfo )
{
	const std::string TargetNodeName = AiNode->mName.C_Str();
	auto TargetNode = std::make_shared<Node>();
	auto iter = BoneTableParserInfo.find(TargetNodeName);
	const bool bParserInfo =
			std::end(BoneTableParserInfo) != iter;

	if (bParserInfo)
	{
		const auto& ParserInfo = iter->second;
		TargetNode->Index = ParserInfo.first;
		TargetNode->Offset = ParserInfo.second;
	}
	else
	{
		TargetNode->Index = -1;
		TargetNode->Offset = FMath::Identity();
	}
	
	TargetNode->OriginTransform = TargetNode->Transform =
		AssimpHelper::ConvertMatrix(AiNode->mTransformation);
	TargetNode->Parent = BoneParent;
	const Matrix ParentToRoot = BoneParent ? 
		BoneParent->ToRoot : FMath::Identity();
	TargetNode->ToRoot = TargetNode->OriginTransform * ParentToRoot;
	TargetNode->Name=TargetNodeName;
	Nodes.insert({ TargetNodeName , TargetNode });

	for (uint32 i = 0; i < AiNode->mNumChildren; ++i)
	{
		const auto*const CurChildren=AiNode->mChildren[i];

		auto ChildrenResult = MakeHierarchy(
				TargetNode.get(),
				CurChildren,
				BoneTableParserInfo);

		TargetNode->Childrens.push_back(ChildrenResult);
	}

	auto ReturnThis =TargetNode.get();
	return ReturnThis;
}

 Node* SkeletonMesh::MakeHierarchyForclones(
	 Node* const Parent,
	 const Node*const SpProtoNode)
 {
	 if (nullptr == SpProtoNode)
	 {
		 PRINT_LOG(L"Warning!", L"Nodes information deep copy error");
	     return nullptr;
	 }
		

	 auto CloneNode = std::make_shared<Node>(*SpProtoNode);
	 CloneNode->Parent = Parent;
	 CloneNode->Childrens.clear();
	 auto Self = CloneNode.get();

	 for (const auto& ProtoChildren : SpProtoNode->Childrens)
	 {
		 auto*const ChildrenRawPtr=MakeHierarchyForclones(Self, ProtoChildren);
		 if (ChildrenRawPtr)
		 {
			 CloneNode->Childrens.push_back(ChildrenRawPtr);
		 }
	 }
	 Nodes.insert({ CloneNode->Name,CloneNode });

	 return Self;
 };

 void SkeletonMesh::InitTextureForVertexTextureFetch()&
 {
	 // 텍셀하나당 flaot4 개씩 저장하므로 매트릭스 하나에는 텍셀 4개가 필요하다.
	 const float TexPitchPrecision = std::sqrtf(BoneSkinningMatries.size() * 4u);
	 const uint8 PowerOfMax = 9u;
	 // 2^9 * 2^9 / 4 = 4096개의 행렬을 저장 가능하며 4096개의 본을 가진 캐릭터가 존재하는 게임을 나는 아직 못만듬.
	 
	 // 가장가까운 2의 승수를 찾는다.
	 for (uint8 PowerOf2 = 0u; PowerOf2 < PowerOfMax; ++PowerOf2)
	 {
		 VTFPitch = std::powl(2, PowerOf2);
		 if (VTFPitch >= TexPitchPrecision)
			 break;
	 }
	 
	 m_pDevice->CreateTexture
	 (VTFPitch, VTFPitch, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED,
		 &BoneAnimMatrixInfo, nullptr);
 }

 void SkeletonMesh::AnimationNotify()&
 {
	 const float AnimDurationNormalize = CurrentAnimMotionTime / (*AnimInfoTable)[AnimName].Duration;

	 auto EventIter =  CurAnimNotify.Event.lower_bound(AnimDurationNormalize);

	 bool bTiming = (std::end(CurAnimNotify.Event) != EventIter) &&
		 AnimDurationNormalize >= EventIter->first;

	 if (bTiming)
	 {
		 auto EventCall = EventIter->second;
		 if (EventCall)
		 {
			const bool bDelete= EventCall();
			if (bDelete)
			{
				CurAnimNotify.Event.erase(EventIter->first);
			}
			
		 }
	 }
 }

