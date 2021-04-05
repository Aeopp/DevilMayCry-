#include "AssimpHelper.hpp"
#include "SkeletonMesh.h"
#include "Subset.h"

USING(ENGINE)

SkeletonMesh::SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: StaticMesh(_pDevice)
{
}

<<<<<<< HEAD
SkeletonMesh::SkeletonMesh(const StaticMesh& _rOther)
	: StaticMesh(_rOther)
{
=======
SkeletonMesh::SkeletonMesh(const SkeletonMesh& _rOther)
	: StaticMesh(_rOther) ,
	bHasAnimation{ _rOther.bHasAnimation},
	RootNodeName{ _rOther  .RootNodeName },
	AnimInfoTable{ _rOther.AnimInfoTable },
	BoneSkinningMatries{ _rOther.BoneSkinningMatries } ,
	VTFPitch{ _rOther.VTFPitch }  ,
	Nodes{ _rOther.Nodes }
{

}

void SkeletonMesh::AnimationEditor()&
{
	if (AnimInfoTable)
	{
		if (ImGui::Button("Save"))
		{
			AnimationSave(ResourcePath);
		}

		for (auto& AnimInfoIter : *AnimInfoTable)
		{
			auto& AnimInfo = AnimInfoIter.second;
			if (ImGui::TreeNode(AnimInfo.Name.c_str()))
			{
				ImGui::BulletText("Duration : %2.3f", AnimInfo.Duration);
				ImGui::BulletText("TickPerSecond : %3.3f", AnimInfo.TickPerSecond);
				ImGui::SliderFloat("Acceleration", &AnimInfo.RefOriginAcceleration(), 0.1f, 10.f);
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
					CurPlayAnimInfo = AnimInfo;
				}
				else
				{
					if ((ImGui::Button("Play")))
					{
						PlayAnimation(AnimInfo.Name,
							true, {});
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
	if (Nodes)
	{
		if (auto RootNodeIter = Nodes->find(RootNodeName);
			RootNodeIter != std::end(*Nodes))
		{
			auto SpNode = RootNodeIter->second;
			if (SpNode)
			{
				SpNode->Editor();
			}
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
	Root->NodeUpdate(FMath::Identity(), CurrentAnimMotionTime,AnimName, IsAnimationBlend);
	VTFUpdate();

	if (CurrentAnimMotionTime > CurPlayAnimInfo.Duration)
	{
		AnimationEnd();
	}
}

void SkeletonMesh::AnimationSave(
	const std::filesystem::path& FullPath)&
{
	if (!AnimInfoTable) return;

	using namespace rapidjson;

	StringBuffer StrBuf{};
	PrettyWriter<StringBuffer> Writer(StrBuf);
	Writer.StartObject();
	Writer.Key("AnimationData");
	Writer.StartArray();
	{
		for (const auto& [AnimName, AnimInfo] : *AnimInfoTable)
		{
			Writer.StartObject();
			{
				Writer.Key("Name");
				Writer.String(AnimName.c_str());

				Writer.Key("Acceleration");
				Writer.Double(AnimInfo.GetOriginAcceleration());

				Writer.Key("TransitionTime");
				Writer.Double(AnimInfo.TransitionTime);
			}
			Writer.EndObject();
		}
	}
	Writer.EndArray();
	Writer.EndObject();

	std::filesystem::path AnimPath = FullPath;
	AnimPath.replace_extension("Animation");
	std::ofstream Of{ AnimPath };
	Of << StrBuf.GetString();
}

void SkeletonMesh::AnimationLoad(
	const std::filesystem::path& FullPath)&
{
	using namespace rapidjson;

	std::filesystem::path AnimPath = FullPath;
	AnimPath.replace_extension("Animation");
	std::ifstream Is{ AnimPath };

	if (!Is.is_open())
		return;

	IStreamWrapper Isw(Is);
	Document _Document{};
	_Document.ParseStream(Isw);

	
	if (_Document.HasParseError())
	{
		PRINT_LOG(L"Warning!", L"Animation Parse Error!");
		return;
	}

	const Value& AnimJsonTable = _Document["AnimationData"];
	const auto& AnimTableArray = AnimJsonTable.GetArray();
	for (auto iter = AnimTableArray.Begin();
		iter != AnimTableArray.end(); ++iter)
	{
		
		AnimInfoTable->find(AnimName);

		if (iter->HasMember("Name"))
		{
			if (false == AnimInfoTable->contains(AnimName))
			{
				// PRINT_LOG(L"Warning!!",L"모델 애니메이션 이름과 데이터 테이블 이름이 매칭되지 않음. 애니메이션 이름이 바뀌었는지 확인해보세요.")
			}

			const std::string AnimName = 
				iter->FindMember("Name")->value.GetString();
			
			(*AnimInfoTable)[AnimName].SetAcceleration(iter->FindMember("Acceleration")->value.GetFloat());
			(*AnimInfoTable)[AnimName].TransitionTime = iter->FindMember("TransitionTime")->value.GetFloat();
		}
	}
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
}

void SkeletonMesh::Free()
{
	StaticMesh::Free();
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
<<<<<<< HEAD
=======

	if (pClone->bClone)
	{
		pClone->InitTextureForVertexTextureFetch();
		//pClone->Nodes->clear();
		//pClone->MakeHierarchyForclones(nullptr, GetRootNode());
	}

>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
	return pClone;
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

<<<<<<< HEAD
	return LoadSkeletonMeshImplementation(AiScene, _Path);


}

HRESULT SkeletonMesh::LoadSkeletonMeshImplementation(const aiScene* AiScene, const std::filesystem::path _Path)
=======
	return LoadSkeletonMeshImplementation(AiScene, ResourcePath);
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


	CurrentAnimMotionTime += (DeltaTime * CurPlayAnimInfo.CalcAcceleration());
	PrevAnimMotionTime += (DeltaTime * PrevPlayAnimInfo.CalcAcceleration());
	TransitionRemainTime -= DeltaTime;
	AnimationUpdateImplementation();
}

void SkeletonMesh::VTFUpdate()&
{
	if (Nodes)
	{
		if (PrevBoneAnimMatrixInfo)
		{
			std::swap(BoneAnimMatrixInfo, PrevBoneAnimMatrixInfo);
		}

		for (const auto& [NodeName, CurNode] : *Nodes)
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
	
}

Node* SkeletonMesh::GetRootNode()&
{
	return GetNode(RootNodeName);
}

Node* SkeletonMesh::GetNode(const std::string& NodeName)&
{
	if (Nodes)
	{
		auto iter = Nodes->find(NodeName);
		if (iter != std::end(*Nodes))
		{
			if (iter->second)
			{
				return iter->second.get();
			}
		}
	}


	return nullptr;
}

void SkeletonMesh::PlayAnimation(
	const std::string& InitAnimName, 
	const bool  bLoop,
	const AnimNotify& _Notify)
{
	if (!AnimInfoTable)return;

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
	PrevPlayAnimInfo = CurPlayAnimInfo;
	CurPlayAnimInfo = iter->second;
	TransitionDuration = TransitionRemainTime = CurPlayAnimInfo.TransitionTime;
	CurAnimNotify = _Notify;
	
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
		PlayAnimation(AnimName,  bLoop, CurAnimNotify);
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
	PrevAnimMotionTime += AnimDelta * PrevPlayAnimInfo.CalcAcceleration(); 
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
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
{
	//Subset을 보관하는 vector 메모리 공간 확보.
	m_vecSubset.resize(AiScene->mNumMeshes);

	//FBX의 Scene을 구성하는 Mesh(Subset)로 부터 데이터 구성.
	for (uint32 MeshIdx = 0u; MeshIdx < AiScene->mNumMeshes; ++MeshIdx)
	{
		//
		const auto* const AiMesh = AiScene->mMeshes[MeshIdx];
		//
		std::shared_ptr<Subset> _CurrentSubset;
		_CurrentSubset.reset(Subset::Create(m_pDevice), Deleter<Object>());

		VERTEXBUFFERDESC tVBDesc;
		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
		LPDIRECT3DINDEXBUFFER9	pIB = nullptr;

		if (FAILED(AssimpHelper::LoadMesh(AiMesh, m_pDevice,
			&tVBDesc, &pVB, &pIB)))
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

<<<<<<< HEAD
=======
	std::set<std::string> BoneNameSet{};
	for (const auto&  [BoneName,Info] :BoneTableParserInfo)
	{
		BoneNameSet.insert(BoneName);
	}

	BoneSkinningMatries.resize(BoneTableParserInfo.size());

	RootNodeName = AiScene->mRootNode->mName.C_Str();
	Nodes = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Node>>>();
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
			CurAnimInfo.SetAcceleration(1.0f);
			CurAnimInfo.Name = _AiAnimation->mName.C_Str();
			CurAnimInfo.Duration = _AiAnimation->mDuration;

			for (uint32 ChannelIdx = 0u;
				ChannelIdx < _AiAnimation->mNumChannels; ++ChannelIdx)
			{
				const auto& CurChannelkey = _AiAnimation->mChannels[ChannelIdx];
				const std::string NodeName =CurChannelkey->mNodeName.C_Str();
				auto iter = Nodes->find(NodeName);
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

>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
	MakeVertexLcationsFromSubset();

	return S_OK;

	
}

Bone* SkeletonMesh::MakeHierarchy(Bone* BoneParent, const aiNode* const AiNode)
{
	auto TargetBone = std::make_shared<Bone>();
	BoneTable.push_back(TargetBone);
	const uint64 CurBoneIdx = BoneTable.size() - 1;
	const std::string InsertBoneName = AiNode->mName.C_Str();
	// 성공하면 이름 있을 경우 이름 변경해서 삽입하는 로직 추가 .
	if (BoneTableIdxFromName.contains(InsertBoneName))
	{

	}
<<<<<<< HEAD
	BoneTableIdxFromName.insert({ AiNode->mName.C_Str()  ,CurBoneIdx });
	TargetBone->Name = AiNode->mName.C_Str();
	TargetBone->OriginTransform = TargetBone->Transform = AssimpHelper::ConvertMatrix(AiNode->mTransformation);
	TargetBone->Parent = BoneParent;
	TargetBone->ToRoot = TargetBone->OriginTransform * BoneParent->ToRoot;
=======
	
	TargetNode->OriginTransform = TargetNode->Transform =
		AssimpHelper::ConvertMatrix(AiNode->mTransformation);
	TargetNode->Parent = BoneParent;
	const Matrix ParentToRoot = BoneParent ? 
		BoneParent->ToRoot : FMath::Identity();
	TargetNode->ToRoot = TargetNode->OriginTransform * ParentToRoot;
	TargetNode->Name=TargetNodeName;
	Nodes->insert({ TargetNodeName , TargetNode });
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0

	for (uint32 i = 0; i < AiNode->mNumChildren; ++i)
	{
		TargetBone->Childrens.push_back(MakeHierarchy(TargetBone.get(), AiNode->mChildren[i]));
	}

	return TargetBone.get();
}
<<<<<<< HEAD
=======

 //Node* SkeletonMesh::MakeHierarchyForclones(
	// Node* const Parent,
	// const Node*const SpProtoNode)
 //{
	// if (nullptr == SpProtoNode)
	// {
	//	 PRINT_LOG(L"Warning!", L"Nodes information deep copy error");
	//     return nullptr;
	// }

	// auto CloneNode = std::make_shared<Node>(*SpProtoNode);
	// CloneNode->Parent = Parent;
	// CloneNode->Childrens.clear();
	// auto Self = CloneNode.get();

	// for (const auto& ProtoChildren : SpProtoNode->Childrens)
	// {
	//	 auto*const ChildrenRawPtr=MakeHierarchyForclones(Self, ProtoChildren);
	//	 if (ChildrenRawPtr)
	//	 {
	//		 CloneNode->Childrens.push_back(ChildrenRawPtr);
	//	 }
	// }
	// Nodes.insert({ CloneNode->Name,CloneNode });

	// return Self;
 //};

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

>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
