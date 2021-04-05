#include "imgui.h"
#include "Node.h"
USING(ENGINE)


void Node::Editor()&
{
	if (ImGui::TreeNode(Name.c_str()))
	{
		ImGui::TreePop();

		for (auto& Children : Childrens)
		{
			Children->Editor();
		}
	}
}

std::tuple<Vector3,Quaternion,Vector3>
		CurrentAnimationTransform(
		const AnimationTrack& AnimTrack , 
		const double CurrentAnimationTime)
{
	Vector3 Scale{ 1,1,1 };
	Quaternion Quat{};
	Vector3 Pos{ 0,0,0 };

	{
		auto Right = AnimTrack.ScaleTimeLine.upper_bound(CurrentAnimationTime);

		if (Right == AnimTrack.ScaleTimeLine.begin())
		{
			Scale = Right->second;
		}
		else if (Right == AnimTrack.ScaleTimeLine.end())
		{
			std::advance(Right, -1);
			Scale = Right->second;
		}
		else
		{
			auto Left = Right;
			std::advance(Left, -1);
			const double Interval = Right->first - Left->first;
			const float t = (CurrentAnimationTime - Left->first) / Interval;
			D3DXVec3Lerp(&Scale, &Left->second, &Right->second, t);
		}
	}

	{
		auto Right = AnimTrack.QuatTimeLine.upper_bound(CurrentAnimationTime);

		if (Right == AnimTrack.QuatTimeLine.begin())
		{
			Quat = Right->second;
		}
		else if (Right == AnimTrack.QuatTimeLine.end())
		{
			std::advance(Right, -1);
			Quat = Right->second;
		}
		else
		{
			auto Left = Right;
			std::advance(Left, -1);
			const double Interval = Right->first - Left->first;
			const float t = (CurrentAnimationTime - Left->first) / Interval;
			D3DXQuaternionSlerp
			(&Quat, &Left->second, &Right->second, t);
		}
	}

	{
		auto Right = AnimTrack.PosTimeLine.upper_bound(CurrentAnimationTime);

		if (Right == AnimTrack.PosTimeLine.begin())
		{
			Pos = Right->second;
		}
		else if (Right == AnimTrack.PosTimeLine.end())
		{
			std::advance(Right, -1);
			Pos = Right->second;
		}
		else
		{
			auto Left = Right;
			std::advance(Left, -1);
			const double Interval = Right->first - Left->first;
			const float t = (CurrentAnimationTime - Left->first) / Interval;
			D3DXVec3Lerp
			(&Pos, &Left->second, &Right->second, t);
		}
	}

	return {Scale,Quat,Pos};
}

void Node::NodeUpdate(const Matrix& ParentToRoot,
				     const double CurrentAnimationTime,
				     const std::string& AnimationName, 
	const std::optional<AnimationBlendInfo>& IsAnimationBlend)&
{
	// ���⼭ ���� �����Ӱ� ���� �������� ���� �Ѵ�.
	
	auto iter = _AnimationTrack.find(AnimationName);
	const bool bCurAnim = iter != std::end(_AnimationTrack);

	if (bCurAnim)
	{
		auto [Scale,Quat,Pos ] = CurrentAnimationTransform(iter->second, CurrentAnimationTime);

		if (IsAnimationBlend.has_value())
		{
			auto PrevIter = 
				_AnimationTrack.find(IsAnimationBlend->PrevAnimationName);
			const bool bPrevAnim = PrevIter != std::end(_AnimationTrack);
			if (bPrevAnim)
			{
				auto [PrevScale, PrevQuat, PrevPos] =
					CurrentAnimationTransform(PrevIter->second,
					IsAnimationBlend->AnimationTime);

				const double BlendWeight =
					1.0 - IsAnimationBlend->PrevAnimationWeight;

				D3DXVec3Lerp(&Scale, &PrevScale, &Scale, BlendWeight);
				D3DXQuaternionSlerp(&Quat, &PrevQuat, &Quat, BlendWeight);
				D3DXVec3Lerp(&Pos, &PrevPos, &Pos, BlendWeight);
			}
		}
			Transform = FMath::Scale(Scale) *
					FMath::Rotation(Quat) *
					FMath::Translation(Pos);
	}
	else
	{
		Transform = OriginTransform;
	}

	ToRoot = Transform * ParentToRoot;
	Final = Offset * ToRoot;

	for (auto& ChildrenTarget : Childrens)
	{
		ChildrenTarget->NodeUpdate(ToRoot,
			CurrentAnimationTime, 
			AnimationName,
			IsAnimationBlend);
	}
}


//
//
//void Engine::Bone::BoneEdit()
//{
//	if (ImGui::TreeNode(Name.c_str()))
//	{
//		bEditObserver = true;
//		if (ImGui::CollapsingHeader("Edit"))
//		{
//			bEditSelect = true;
//			ImGui::Button("Attach");
//			ImGui::Button("Detach");
//		}
//		else
//		{
//			bEditSelect = false;
//		}
//
//		for (const auto& _Children : Childrens)
//		{
//			_Children->BoneEdit();
//		}
//		ImGui::TreePop();
//	}
//	else
//	{
//		bEditObserver = false;
//	}
//};
//
//void Engine::Bone::DebugRender(
//	const Matrix& World,
//	IDirect3DDevice9* Device,
//	ID3DXMesh* const DebugMesh)&
//{
//	IDirect3DTexture9* CurColorTex{ nullptr };
//	auto& ResourceSys = ResourceSystem::Instance;
//
//	if (bEditSelect)
//	{
//		CurColorTex = ResourceSys->Get<IDirect3DTexture9>(L"Texture_Red");
//	}
//	else if (bEditObserver)
//	{
//		CurColorTex = ResourceSys->Get<IDirect3DTexture9>(L"Texture_Blue");
//	}
//	else
//	{
//		CurColorTex = ResourceSys->Get<IDirect3DTexture9>(L"Texture_Green");
//	}
//
//	const Matrix ToRootWorld = ToRoot * World;
//	const Matrix Final = FMath::Scale({ 5.f,5.f,5.f }) * ToRootWorld;
//	Device->SetTransform(D3DTS_WORLD, &Final);
//	Device->SetTexture(0, CurColorTex);
//	DebugMesh->DrawSubset(0);
//
//	if (CollisionGeometric)
//	{
//		Device->SetTransform(D3DTS_WORLD, &ToRootWorld);
//		CollisionGeometric->Render(Device, true);
//	}
//};
