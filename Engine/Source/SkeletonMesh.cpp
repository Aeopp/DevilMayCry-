#include "AssimpHelper.hpp"
#include "SkeletonMesh.h"
#include "Subset.h"
#include "Renderer.h"

#include <optional>
#include <fstream>
#include <ostream>


USING(ENGINE)

SkeletonMesh::SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: StaticMesh(_pDevice)
{

}

SkeletonMesh::SkeletonMesh(const SkeletonMesh& _rOther)
	: StaticMesh(_rOther),
	bHasAnimation{ _rOther.bHasAnimation },
	RootNodeName{ _rOther.RootNodeName },
	AnimInfoTable{ _rOther.AnimInfoTable },
	VTFPitch{ _rOther.VTFPitch },
	Nodes{ _rOther.Nodes },
	AnimIndexNameMap{ _rOther.AnimIndexNameMap },
	RootMotionScaleName{ _rOther.RootMotionScaleName },
	RootMotionTransitionName{ _rOther.RootMotionTransitionName },
	bRootMotionScale{ _rOther.bRootMotionScale },
	bRootMotionRotation{ _rOther.bRootMotionRotation },
	bRootMotionTransition{ _rOther.bRootMotionTransition },
	RootMotionDeltaFactor{ _rOther.RootMotionDeltaFactor } 
{
	BoneSkinningMatries.resize(_rOther.BoneSkinningMatries.size());
}

void SkeletonMesh::AnimationEditor()&
{
	if (AnimInfoTable)
	{
		if (ImGui::BeginPopup("Really Save?"))
		{
			ImGui::Text("If the file exists, the previous information will be blown away.");
			if (ImGui::SmallButton("Yes"))
			{
				AnimationSave(ResourcePath);
				ImGui::CloseCurrentPopup();
			}
			else if (ImGui::SmallButton("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Save"))
		{
			ImGui::OpenPopup("Really Save?");
		}
		if (ImGui::TreeNode("RootMotion"))
		{
			ImGui::SliderFloat("RootMotionDeltaFactor", &RootMotionDeltaFactor, 0.000001f, 1.f);
			if (ImGui::TreeNode("Delta"))
			{
				ImGui::BulletText("Delta Scale : %3.3f , %3.3f , %3.3f",
					RootMotionLastCalcDeltaScale.x, RootMotionLastCalcDeltaScale.y, RootMotionLastCalcDeltaScale.z);

				ImGui::BulletText("Delta Quat : %3.3f , %3.3f , %3.3f , %3.3f",
					RootMotionLastCalcDeltaQuat.x, RootMotionLastCalcDeltaQuat.y, RootMotionLastCalcDeltaQuat.z,
					RootMotionLastCalcDeltaQuat.w);

				ImGui::BulletText("Delta Pos : %3.3f , %3.3f , %3.3f",
					RootMotionLastCalcDeltaPos.x, RootMotionLastCalcDeltaPos.y, RootMotionLastCalcDeltaPos.z);

				ImGui::TreePop();
			}

			if (ImGui::Checkbox("RootMotionScale", &bRootMotionScale))
			{
				if (bRootMotionScale)
				{
					EnableScaleRootMotion(RootMotionScaleName);
				}
				else
				{
					DisableScaleRootMotion();
				}
			}

			if (ImGui::Checkbox("RootMotionRotation", &bRootMotionRotation))
			{
				if (bRootMotionRotation)
				{
					EnableRotationRootMotion(RootMotionRotationName);
				}
				else
				{
					DisableTransitionRootMotion();
				}
			}

			if (ImGui::Checkbox("RootMotionTransition", &bRootMotionTransition))
			{
				if (bRootMotionTransition)
				{
					EnableTransitionRootMotion(RootMotionTransitionName);
				}
				else
				{
					DisableTransitionRootMotion();
				}
			}


			ImGui::TreePop();
		}



		for (auto& AnimInfoIter : *AnimInfoTable)
		{
			auto& AnimInfo = AnimInfoIter.second;

			if (ImGui::TreeNode(AnimInfo.Name.c_str()))
			{
				ImGui::BulletText("Duration : %2.3f", AnimInfo.Duration);
				ImGui::BulletText("TickPerSecond : %3.3f", AnimInfo.TickPerSecond);
				ImGui::SliderFloat("Acceleration", &AnimInfo.RefOriginAcceleration(), 0.1f, 10.f);
				ImGui::SliderFloat("TransitionTime", &AnimInfo.TransitionTime, 0.009f, 5.f);

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

				if ((ImGui::Button("Play")))
				{
					PlayAnimation(AnimInfo.Name, true, {});
				};

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

				SpNode->Editor(RootMotionScaleName,
					RootMotionRotationName,
					RootMotionTransitionName);
			}
		}
	}

}

std::tuple<Vector3, Quaternion, Vector3> SkeletonMesh::AnimationUpdateImplementation()&
{
	std::optional<float> bTimeBeyondAnimation{};
	const float TimeBeyondAnimation = (CurrentAnimMotionTime - CurPlayAnimInfo.Duration);
	if (TimeBeyondAnimation > 0.0f)
	{
		bTimeBeyondAnimation = TimeBeyondAnimation;
	}

	AnimationNotify();
	std::optional<AnimationBlendInfo> IsAnimationBlend = std::nullopt;

	if (TransitionRemainTime > 0.0)
	{
		if (AnimInfoTable)
		{
			auto Iter = AnimInfoTable->find(PrevAnimName);
			if (Iter != std::end(*AnimInfoTable))
			{
				if (PrevAnimMotionTime > (*AnimInfoTable)[PrevAnimName].Duration)
				{
					PrevAnimMotionTime = (*AnimInfoTable)[PrevAnimName].Duration;
				}

				AnimationBlendInfo _PrevAnimBlend;
				_PrevAnimBlend.PrevAnimationName = PrevAnimName;
				_PrevAnimBlend.AnimationTime = PrevAnimMotionTime;
				_PrevAnimBlend.PrevAnimationWeight = TransitionRemainTime / TransitionDuration;
				IsAnimationBlend = _PrevAnimBlend;
			}
		}
	}

#pragma region ROOT_MOTION
	std::optional<std::string> IsRootMotion;

	if (bRootMotionTransition)
	{
		RootMotionLastCalcDeltaPos = CalcRootMotionDeltaPos(TimeBeyondAnimation,
			AnimName, CurPlayAnimInfo.Duration, CurrentAnimPrevFrameMotionTime, CurrentAnimMotionTime);

		if (IsAnimationBlend)
		{
			const Vector3 PrevAnimBlendDeltaPos = CalcRootMotionDeltaPos({},
				PrevAnimName, PrevPlayAnimInfo.Duration, PrevAnimPrevFrameMotionTime, PrevAnimMotionTime);

			const double CurBlendWeight =
				1.0 - IsAnimationBlend->PrevAnimationWeight;

			RootMotionLastCalcDeltaPos =
				RootMotionLastCalcDeltaPos * CurBlendWeight +
				PrevAnimBlendDeltaPos * IsAnimationBlend->PrevAnimationWeight;
		}
	}
	else
	{
		RootMotionLastCalcDeltaPos = { 0,0,0 };
	};


	if (bRootMotionRotation)
	{
		RootMotionLastCalcDeltaQuat = CalcRootMotionDeltaQuat(TimeBeyondAnimation,
			AnimName, CurPlayAnimInfo.Duration, CurrentAnimPrevFrameMotionTime, CurrentAnimMotionTime);

		if (IsAnimationBlend)
		{
			const Quaternion PrevAnimBlendDeltaQuat = CalcRootMotionDeltaQuat({},
				PrevAnimName, PrevPlayAnimInfo.Duration, PrevAnimPrevFrameMotionTime, PrevAnimMotionTime);

			const double CurBlendWeight =
				1.0 - IsAnimationBlend->PrevAnimationWeight;

			RootMotionLastCalcDeltaQuat =
				RootMotionLastCalcDeltaQuat * CurBlendWeight +
				PrevAnimBlendDeltaQuat * IsAnimationBlend->PrevAnimationWeight;
		}
	}
	else
	{
		RootMotionLastCalcDeltaQuat = { 0,0,0 ,1 };
	}

	if (bRootMotionScale)
	{
		RootMotionLastCalcDeltaScale = CalcRootMotionDeltaScale(TimeBeyondAnimation,
			AnimName, CurPlayAnimInfo.Duration, CurrentAnimPrevFrameMotionTime, CurrentAnimMotionTime);

		if (IsAnimationBlend)
		{
			const Vector3 PrevAnimBlendDeltaScale = CalcRootMotionDeltaScale({},
				PrevAnimName, PrevPlayAnimInfo.Duration, PrevAnimPrevFrameMotionTime, PrevAnimMotionTime);

			const double CurBlendWeight =
				1.0 - IsAnimationBlend->PrevAnimationWeight;

			RootMotionLastCalcDeltaScale =
				RootMotionLastCalcDeltaScale * CurBlendWeight +
				PrevAnimBlendDeltaScale * IsAnimationBlend->PrevAnimationWeight;
		}
	}
	else
	{
		RootMotionLastCalcDeltaScale = { 0,0,0 };
	}


#pragma endregion ROOT_MOTION



	// 루트모션 종료.....
	auto* const Root = GetRootNode();
	// 노드 정보를 클론들끼리 공유하기 때문에 업데이트 직후 반드시 VTF Update 수행...
	const float CurPlayAnimMotionTime = bTimeBeyondAnimation.has_value()
		? bTimeBeyondAnimation.value() : CurrentAnimMotionTime;

	Root->NodeUpdate(FMath::Identity(),
		CurPlayAnimMotionTime, AnimName, IsAnimationBlend);
	//
	VTFUpdate();

	if (bTimeBeyondAnimation)
	{
		AnimationEnd();
	}

	return { RootMotionLastCalcDeltaScale
			* RootMotionDeltaFactor,
			RootMotionLastCalcDeltaQuat
			* RootMotionDeltaFactor,
			RootMotionLastCalcDeltaPos
		   * RootMotionDeltaFactor };
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
	Writer.StartObject();
	{
		Writer.Key("RootMotion_DeltaFactor");
		Writer.Double(RootMotionDeltaFactor);

		Writer.Key("RootMotion_ScaleName");
		Writer.String(RootMotionScaleName.c_str());

		Writer.Key("RootMotion_RotationName");
		Writer.String(RootMotionRotationName.c_str());

		Writer.Key("RootMotion_TransitionName");
		Writer.String(RootMotionTransitionName.c_str());

		Writer.Key("AnimInfoTable");
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
	}
	Writer.EndObject();




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
	if (AnimJsonTable.IsObject())
	{
		if (AnimJsonTable.HasMember("AnimInfoTable"))
		{
			auto AnimTableArray = AnimJsonTable.FindMember("AnimInfoTable")->value.GetArray();

			for (auto iter = AnimTableArray.Begin();
				iter != AnimTableArray.end(); ++iter)
			{

				AnimInfoTable->find(AnimName);

				if (iter->HasMember("Name"))
				{
					if (false == AnimInfoTable->contains(AnimName))
					{
						Log("No model animation name and no data table name occur. Check if the animation has been renamed");
						// PRINT_LOG(L"Warning!!",L".")
					}

					const std::string AnimName =
						iter->FindMember("Name")->value.GetString();

					(*AnimInfoTable)[AnimName].SetAcceleration(iter->FindMember("Acceleration")->value.GetFloat());
					(*AnimInfoTable)[AnimName].TransitionTime = iter->FindMember("TransitionTime")->value.GetFloat();
				}
			}
		}



		{
			if (AnimJsonTable.HasMember("RootMotion_DeltaFactor"))
			{
				RootMotionDeltaFactor = AnimJsonTable.FindMember("RootMotion_DeltaFactor")->value.GetDouble();
			}

			if (AnimJsonTable.HasMember("RootMotion_ScaleName"))
			{
				RootMotionScaleName= AnimJsonTable.FindMember("RootMotion_ScaleName")->value.GetString();
				EnableScaleRootMotion(RootMotionScaleName);
			}

			if (AnimJsonTable.HasMember("RootMotion_RotationName"))
			{
				RootMotionRotationName= AnimJsonTable.FindMember("RootMotion_RotationName")->value.GetString();
				EnableRotationRootMotion(RootMotionRotationName);
			}

			if (AnimJsonTable.HasMember("RootMotion_TransitionName"))
			{
				RootMotionTransitionName= AnimJsonTable.FindMember("RootMotion_TransitionName")->value.GetString();
				EnableTransitionRootMotion(RootMotionTransitionName);
			}
		};


	}
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
	const std::filesystem::path _Path,
	const std::any& InitParams)
{
	SkeletonMesh* pInstance = new SkeletonMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_Path, InitParams)))
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
		//pClone->Nodes->clear();
		//pClone->MakeHierarchyForclones(nullptr, GetRootNode());
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
};

std::string SkeletonMesh::GetName()
{
	return "SkeletonMesh";
};

void SkeletonMesh::BindVTF(ID3DXEffect* Fx)&
{
	Fx->SetTexture("VTF", BoneAnimMatrixInfo);
	Fx->SetInt("VTFPitch", VTFPitch);
};

bool SkeletonMesh::IsAnimationEnd()
{
	return bAnimationEnd;
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
};

std::tuple<Vector3, Quaternion, Vector3> SkeletonMesh::Update(const float DeltaTime)&
{
	if (bAnimationEnd || bAnimStop)return { {0,0,0},{0,0,0,1},{0,0,0} };

	const float CalcDeltaTime = DeltaTime * DeltaTimeFactor;

	CurrentAnimPrevFrameMotionTime = CurrentAnimMotionTime;

	CurrentAnimMotionTime +=
		(CalcDeltaTime *
			CurPlayAnimInfo.CalcAcceleration(CurrentAccelerationFactor));

	PrevAnimPrevFrameMotionTime = PrevAnimMotionTime;

	PrevAnimMotionTime += (CalcDeltaTime *
		PrevPlayAnimInfo.CalcAcceleration(PrevAccelerationFactor));

	TransitionRemainTime -= (CalcDeltaTime * CurrentTransitionTimeFactor);

	return AnimationUpdateImplementation();
};

void SkeletonMesh::BoneDebugRender(
	const Matrix& OwnerTransformWorld,
	ID3DXEffect* const Fx)&
{
	static auto DebugSphereMesh = Resources::Load<ENGINE::StaticMesh>(
		"..\\..\\Resource\\Mesh\\Static\\Sphere.fbx", {});

	if (!Nodes || !DebugSphereMesh || bAnimationEnd) return;

	Log("Bone Debug Render : Uninitialized nodes !");

	for (auto& [NodeName, _Node] : *Nodes)
	{
		if (auto OToRoot = GetNodeToRoot(NodeName);
			OToRoot)
		{
			const Matrix ToRoot = OToRoot.value();
			Fx->SetMatrix("ToRoot", &ToRoot);
			DebugSphereMesh->Render(Fx);
		}
	}
};

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
			if (CurNode->IsBone())
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
};

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

std::optional<Matrix> SkeletonMesh::GetNodeToRoot(const std::string& NodeName)&
{
	if (Nodes)
	{
		auto iter = Nodes->find(NodeName);
		if (iter != std::end(*Nodes))
		{
			auto SpNode = iter->second;
			const uint32 NodeIndex = SpNode->Index;
			// if (SpNode->IsBone())
			{
				return SpNode->ToRoot;
				// return   FMath::Inverse(SpNode->Offset) * BoneSkinningMatries[NodeIndex];
			}
		}
	}

	return std::nullopt;
}

void SkeletonMesh::PlayAnimation(
	const std::string& InitAnimName,
	const bool  bLoop,
	const AnimNotify& _Notify,
	const float _CurrentAccelerationFactor,
	const float _CurrentTransitionTimeFactor)
{
	if (!AnimInfoTable)return;
	//    같은 모션 일경우 빠른 리턴.
	if (InitAnimName == CurPlayAnimInfo.Name) return;

	auto iter = AnimInfoTable->find(InitAnimName);
	if (iter == std::end(*AnimInfoTable))
	{
		return;
	}
	bAnimStop = false;
	bAnimationEnd = false;
	this->bLoop = bLoop;
	PrevAnimMotionTime = CurrentAnimMotionTime;
	CurrentAnimMotionTime = 0.0;
	PrevAccelerationFactor = CurrentAccelerationFactor;
	CurrentAccelerationFactor = _CurrentAccelerationFactor;
	PrevAnimName = AnimName;
	AnimName = InitAnimName;
	PrevPlayAnimInfo = CurPlayAnimInfo;
	CurPlayAnimInfo = iter->second;
	TransitionDuration = TransitionRemainTime = CurPlayAnimInfo.TransitionTime * _CurrentTransitionTimeFactor;
	CurAnimNotify = _Notify;
}

void SkeletonMesh::PlayAnimation(const uint32 AnimationIndex, const bool bLoop, const AnimNotify& _Notify,
	const float _CurrentAccelerationFactor,
	const float _CurrentTransitionTimeFactor)
{
	if (AnimIndexNameMap)
	{
		if (auto iter = AnimIndexNameMap->find(AnimationIndex);
			iter != std::end(*AnimIndexNameMap))
		{
			PlayAnimation(iter->second, bLoop, _Notify,
				_CurrentAccelerationFactor, _CurrentTransitionTimeFactor);
		}
	}
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
};

void SkeletonMesh::AnimationEnd()&
{
	CurrentAnimPrevFrameMotionTime = 0.0f;

	if (bLoop)
	{
		CurrentAnimMotionTime -= CurPlayAnimInfo.Duration;
		PrevAnimMotionTime = CurPlayAnimInfo.Duration;
		TransitionDuration = CurPlayAnimInfo.TransitionTime;
		bAnimationEnd = false;
		bAnimStop = false;
		PrevAccelerationFactor = CurrentAccelerationFactor;
		PrevAnimName = AnimName;
		PrevPlayAnimInfo = CurPlayAnimInfo;
	}
	else
	{
		bAnimationEnd = true;
	}
};

float SkeletonMesh::PlayingTime()
{
	if (bAnimationEnd)
		return 0.0f;

	return CurrentAnimMotionTime / CurPlayAnimInfo.Duration;
}

void SkeletonMesh::SetPlayingTime(float NewTime)
{
	if (bAnimationEnd)return;
	NewTime = std::clamp(NewTime, 0.0f, 1.0f);
	const float AnimDelta = NewTime - PlayingTime();
	const float SetTime = NewTime * CurPlayAnimInfo.Duration;
	CurrentAnimPrevFrameMotionTime = CurrentAnimMotionTime;
	CurrentAnimMotionTime = SetTime;
	PrevAnimPrevFrameMotionTime = PrevAnimMotionTime;
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
		if (iter != std::end(*AnimInfoTable))
		{
			return iter->second;
		}
	}

	return std::nullopt;
}

void SkeletonMesh::SetDeltaTimeFactor(const float DeltaTimeFactor)
{
	this->DeltaTimeFactor = DeltaTimeFactor;
}

static aiNode* FindBone(aiNode* AiNode,
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


HRESULT SkeletonMesh::LoadMeshImplementation(
	const aiScene* AiScene,
	const std::filesystem::path _Path,
	const std::any& InitParams)
{
	Mesh::InitializeInfo _InitInfo{};
	if (InitParams.has_value())
	{
		_InitInfo = std::any_cast<Mesh::InitializeInfo>(InitParams);
	}

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
			&tVBDesc, &pVB, &pIB, &BoneTableParserInfo, _InitInfo.bLocalVertexLocationsStorage)))
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
	for (const auto& [BoneName, Info] : BoneTableParserInfo)
	{
		BoneNameSet.insert(BoneName);
	}

	BoneSkinningMatries.resize(BoneTableParserInfo.size());

	RootNodeName = AiScene->mRootNode->mName.C_Str();
	Nodes = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Node>>>();
	MakeHierarchy(nullptr, AiScene->mRootNode, BoneTableParserInfo);

	bHasAnimation = AiScene->HasAnimations();

	if (bHasAnimation)
	{
		AnimIndexNameMap = std::make_shared<std::map<uint32, std::string>>();

		AnimInfoTable = std::make_shared<std::map<std::string, AnimationInformation>>();

		for (uint32 AnimIdx = 0u; AnimIdx < AiScene->mNumAnimations; ++AnimIdx)
		{
			aiAnimation* _AiAnimation = AiScene->mAnimations[AnimIdx];

			AnimationInformation CurAnimInfo{};
			CurAnimInfo.TickPerSecond = _AiAnimation->mTicksPerSecond;
			CurAnimInfo.SetAcceleration(1.0f);
			CurAnimInfo.Name = _AiAnimation->mName.C_Str();
			CurAnimInfo.Duration = _AiAnimation->mDuration;

			(*AnimIndexNameMap)[AnimIdx] = CurAnimInfo.Name;

			for (uint32 ChannelIdx = 0u;
				ChannelIdx < _AiAnimation->mNumChannels; ++ChannelIdx)
			{
				const auto& CurChannelkey = _AiAnimation->mChannels[ChannelIdx];
				const std::string NodeName = CurChannelkey->mNodeName.C_Str();
				auto iter = Nodes->find(NodeName);
				auto SpNode = iter->second;
				auto& CurAnimTrack = SpNode->_AnimationTrack[CurAnimInfo.Name];
				auto& RefScaleTrack = CurAnimTrack.ScaleTimeLine;
				auto& RefQuatTrack = CurAnimTrack.QuatTimeLine;
				auto& RefPosTrack = CurAnimTrack.PosTimeLine;

				for (uint32 ScaleKeyIdx = 0u;
					ScaleKeyIdx < CurChannelkey->mNumScalingKeys;
					++ScaleKeyIdx)
				{
					const auto& CurScaleKey = CurChannelkey->mScalingKeys[ScaleKeyIdx];

					RefScaleTrack.insert({ CurScaleKey.mTime,
						AssimpHelper::ConvertVec3(CurScaleKey.mValue) });
				}

				for (uint32 QuatKeyIdx = 0u;
					QuatKeyIdx < CurChannelkey->mNumRotationKeys;
					++QuatKeyIdx)
				{
					const auto& CurQuatKey = CurChannelkey->mRotationKeys[QuatKeyIdx];
					auto Quat = AssimpHelper::ConvertQuat(CurQuatKey.mValue);
					D3DXQuaternionNormalize(&Quat, &Quat);

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
						AssimpHelper::ConvertVec3(CurPosKey.mValue)
						});
				}
			}

			AnimInfoTable->insert({ CurAnimInfo.Name , CurAnimInfo });
		}
	}

	if (_InitInfo.bLocalVertexLocationsStorage)
	{
		MakeVertexLocationsFromSubset();
	}

	if (bHasAnimation)
	{
		AnimationLoad(_Path);

		//// 여기서  값 다 따로 하기 !! 
		//if (_InitInfo.bRootMotionScale)
		//{
		//	//EnableScaleRootMotion(RootMotionScaleName);
		//}
		//else
		//{
		//	DisableScaleRootMotion();
		//}

		//if (_InitInfo.bRootMotionRotation)
		//{
		//	//EnableRotationRootMotion(RootMotionRotationName);
		//}
		//else
		//{
		//	DisableRotationRootMotion();
		//}

		//if (_InitInfo.bRootMotionTransition)
		//{
		//	//EnableTransitionRootMotion(RootMotionTransitionName);
		//}
		//else
		//{
		//	DisableTransitionRootMotion();
		//}
	};

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
	std::pair<uint32, Matrix>>&BoneTableParserInfo)
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
		TargetNode->Offset = FMath::Identity();
	}

	TargetNode->OriginTransform = TargetNode->Transform =
		AssimpHelper::ConvertMatrix(AiNode->mTransformation);
	TargetNode->Parent = BoneParent;
	const Matrix ParentToRoot = BoneParent ?
		BoneParent->ToRoot : FMath::Identity();
	TargetNode->ToRoot = TargetNode->OriginTransform * ParentToRoot;
	TargetNode->Name = TargetNodeName;
	Nodes->insert({ TargetNodeName , TargetNode });

	for (uint32 i = 0; i < AiNode->mNumChildren; ++i)
	{
		const auto* const CurChildren = AiNode->mChildren[i];

		auto ChildrenResult = MakeHierarchy(
			TargetNode.get(),
			CurChildren,
			BoneTableParserInfo);

		TargetNode->Childrens.push_back(ChildrenResult);
	}

	auto ReturnThis = TargetNode.get();
	return ReturnThis;
}

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
	const float AnimDurationNormalize =
		CurrentAnimMotionTime / (*AnimInfoTable)[AnimName].Duration;

	auto EventIter = CurAnimNotify.Event.lower_bound(AnimDurationNormalize);

	bool bTiming = (std::end(CurAnimNotify.Event) != EventIter) &&
		AnimDurationNormalize >= EventIter->first;

	if (bTiming)
	{
		auto EventCall = EventIter->second;
		if (EventCall)
		{
			const bool bDelete = EventCall();
			if (bDelete)
			{
				CurAnimNotify.Event.erase(EventIter->first);
			}

		}
	}
};

Vector3 SkeletonMesh::CalcRootMotionDeltaPos(
	std::optional<float> bTimeBeyondAnimation,
	const std::string& _TargetAnimName,
	const float AnimDuraion,
	const float AnimPrevFrameMotionTime,
	const float AnimMotionTime)&
{
	auto* const RootMotionPosNode = GetNode(RootMotionTransitionName);
	if (RootMotionPosNode)
	{
		auto iter = RootMotionPosNode->_AnimationTrack.find(_TargetAnimName);
		const bool bCurAnim = iter != std::end(RootMotionPosNode->_AnimationTrack);
		if (bCurAnim)
		{
			const auto& RootAnimTrack = iter->second;

			if (bTimeBeyondAnimation > 0.f)
			{
				const Vector3 EndPos = Node::CurrentAnimationPosition(RootAnimTrack, AnimDuraion);
				const Vector3 PrevPos = Node::CurrentAnimationPosition(RootAnimTrack, AnimPrevFrameMotionTime);
				const Vector3 StartPos = Node::CurrentAnimationPosition(RootAnimTrack, 0.0f);
				const Vector3 CurPos = Node::CurrentAnimationPosition(RootAnimTrack, bTimeBeyondAnimation.value());
				return (EndPos - PrevPos) + (CurPos - StartPos);
				// 0.9 1.3 일 경우   1.0 - 0.9 0.3 - 0.0.하고 0.3으로 프리 초기화
			}
			else
			{
				const Vector3 CurPos = Node::CurrentAnimationPosition(RootAnimTrack, AnimMotionTime);
				const Vector3 PrevPos = Node::CurrentAnimationPosition(RootAnimTrack, AnimPrevFrameMotionTime);

				return CurPos - PrevPos;
			}
		}
	}

	return { 0,0,0 };
}

Vector3 SkeletonMesh::CalcRootMotionDeltaScale(std::optional<float> bTimeBeyondAnimation, const std::string& _TargetAnimName, const float AnimDuraion, const float AnimPrevFrameMotionTime, const float AnimMotionTime)&
{
	auto* const RootMotionScaleNode = GetNode(RootMotionScaleName);
	if (RootMotionScaleNode)
	{
		auto iter = RootMotionScaleNode->_AnimationTrack.find(_TargetAnimName);
		const bool bCurAnim = iter != std::end(RootMotionScaleNode->_AnimationTrack);
		if (bCurAnim)
		{
			const auto& RootAnimTrack = iter->second;

			if (bTimeBeyondAnimation)
			{
				const Vector3 EndScale = Node::CurrentAnimationScale(RootAnimTrack, AnimDuraion);
				const Vector3 PrevScale = Node::CurrentAnimationScale(RootAnimTrack, AnimPrevFrameMotionTime);
				const Vector3 StartScale = Node::CurrentAnimationScale(RootAnimTrack, 0.0f);
				const Vector3 CurScale = Node::CurrentAnimationScale(RootAnimTrack, bTimeBeyondAnimation.value());
				return (EndScale - PrevScale) + (CurScale - StartScale);
			}
			else
			{
				const Vector3 CurScale = Node::CurrentAnimationScale(RootAnimTrack, AnimMotionTime);
				const Vector3 PrevScale = Node::CurrentAnimationScale(RootAnimTrack, AnimPrevFrameMotionTime);

				return CurScale - PrevScale;
			}
		}
	}

	return { 0,0,0 };
}

Quaternion SkeletonMesh::CalcRootMotionDeltaQuat(std::optional<float> bTimeBeyondAnimation, const std::string& _TargetAnimName, const float AnimDuraion, const float AnimPrevFrameMotionTime, const float AnimMotionTime)&
{
	auto* const RootMotionQuatNode = GetNode(RootMotionRotationName);
	if (RootMotionQuatNode)
	{
		auto iter = RootMotionQuatNode->_AnimationTrack.find(_TargetAnimName);
		const bool bCurAnim = iter != std::end(RootMotionQuatNode->_AnimationTrack);
		if (bCurAnim)
		{
			const auto& RootAnimTrack = iter->second;

			if (bTimeBeyondAnimation)
			{
				const Quaternion EndQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, AnimDuraion);
				const Quaternion PrevQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, AnimPrevFrameMotionTime);
				const Quaternion StartQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, 0.0f);
				const Quaternion CurQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, bTimeBeyondAnimation.value());

				return (EndQuat - PrevQuat) + (CurQuat - StartQuat);
			}
			else
			{
				const Quaternion CurQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, AnimMotionTime);
				const Quaternion PrevQuat = Node::CurrentAnimationQuaternion(RootAnimTrack, AnimPrevFrameMotionTime);

				return CurQuat - PrevQuat;
			}
		}
	}

	return { 0,0,0 ,1 };
}

void SkeletonMesh::LoadAnimation(const std::filesystem::path& FilePath)&
{
	auto AiImporter = Assimp::Importer{};

	const aiScene* const AiScene = AiImporter.ReadFile(
		FilePath.string(),
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
		aiProcess_SplitLargeMeshes |
		aiProcess_JoinIdenticalVertices
	);


	bHasAnimation = AiScene->HasAnimations();

	if (bHasAnimation)
	{
		if (!AnimIndexNameMap)
		{
			AnimIndexNameMap = std::make_shared<std::map<uint32, std::string>>();
		}

		if (!AnimInfoTable)
		{
			AnimInfoTable = std::make_shared<std::map<std::string, AnimationInformation>>();
		};

		for (uint32 AnimIdx = 0u; AnimIdx < AiScene->mNumAnimations; ++AnimIdx)
		{
			aiAnimation* _AiAnimation = AiScene->mAnimations[AnimIdx];

			AnimationInformation CurAnimInfo{};
			CurAnimInfo.TickPerSecond = _AiAnimation->mTicksPerSecond;
			CurAnimInfo.SetAcceleration(1.0f);
			CurAnimInfo.Name = FilePath.filename().stem().string();
			CurAnimInfo.Duration = _AiAnimation->mDuration;

			(*AnimIndexNameMap)[AnimIndexNameMap->size() + AnimIdx] = CurAnimInfo.Name;

			for (uint32 ChannelIdx = 0u;ChannelIdx < _AiAnimation->mNumChannels; ++ChannelIdx)
			{
				const auto& CurChannelkey = _AiAnimation->mChannels[ChannelIdx];
				const std::string NodeName = CurChannelkey->mNodeName.C_Str();
				auto iter = Nodes->find(NodeName);
				auto SpNode = iter->second;
				auto& CurAnimTrack = SpNode->_AnimationTrack[CurAnimInfo.Name];
				auto& RefScaleTrack = CurAnimTrack.ScaleTimeLine;
				auto& RefQuatTrack = CurAnimTrack.QuatTimeLine;
				auto& RefPosTrack = CurAnimTrack.PosTimeLine;

				for (uint32 ScaleKeyIdx = 0u;
					ScaleKeyIdx < CurChannelkey->mNumScalingKeys;
					++ScaleKeyIdx)
				{
					const auto& CurScaleKey = CurChannelkey->mScalingKeys[ScaleKeyIdx];

					RefScaleTrack.insert({ CurScaleKey.mTime,
						AssimpHelper::ConvertVec3(CurScaleKey.mValue) });
				}

				for (uint32 QuatKeyIdx = 0u;
					QuatKeyIdx < CurChannelkey->mNumRotationKeys;
					++QuatKeyIdx)
				{
					const auto& CurQuatKey = CurChannelkey->mRotationKeys[QuatKeyIdx];
					auto Quat = AssimpHelper::ConvertQuat(CurQuatKey.mValue);
					D3DXQuaternionNormalize(&Quat, &Quat);

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
						AssimpHelper::ConvertVec3(CurPosKey.mValue)
						});
				}
			}

			AnimInfoTable->insert({ CurAnimInfo.Name , CurAnimInfo });
		}
	}
};

void SkeletonMesh::EnableScaleRootMotion(const std::string& ScalingRootName)
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (NodeName == ScalingRootName)
				{
					RootMotionScaleName = NodeName;
					_Node->RootMotionFlag = 1;
					bRootMotionScale = true;
				}
			}
		}
	
	}
}

void SkeletonMesh::EnableRotationRootMotion(const std::string& RotationRootName)
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (NodeName == RotationRootName)
				{
					RootMotionRotationName = NodeName;
					_Node->RootMotionFlag = 2;
					bRootMotionRotation = true;

				}
			}
		}
	
	}
}

void SkeletonMesh::EnableTransitionRootMotion(const std::string& TransitionRootName)
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (NodeName == TransitionRootName)
				{
					RootMotionTransitionName = NodeName;
					_Node->RootMotionFlag = 3;
					bRootMotionTransition = true;
				}
			}
		}
		
	}
};

void SkeletonMesh::DisableScaleRootMotion()
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (_Node->RootMotionFlag == 1)
				{
					_Node->RootMotionFlag = -1;
				
				}
			}
		}
	}
	bRootMotionScale = false;
}

void SkeletonMesh::DisableRotationRootMotion()
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (_Node->RootMotionFlag == 2)
				{
					_Node->RootMotionFlag = -1;
				}
			}
		}
		
	}
	bRootMotionRotation = false;
}

void SkeletonMesh::DisableTransitionRootMotion()
{
	if (Nodes)
	{
		for (auto& [NodeName, _Node] : *Nodes)
		{
			if (_Node)
			{
				if (_Node->RootMotionFlag == 3)
				{
					_Node->RootMotionFlag = -1;
				}
			}
		}
	}

	bRootMotionTransition = false;
}
