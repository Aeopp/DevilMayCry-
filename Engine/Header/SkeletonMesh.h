#ifndef _SKELETONMESH_H_
#define _SKELETONMESH_H_
#include "StaticMesh.h"
#include "AnimationBlendInfo.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "AnimationInformation.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <map>
#include <functional>
#include <set>

BEGIN(ENGINE)
class ENGINE_DLL SkeletonMesh final : public StaticMesh
{
private:
	explicit SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit SkeletonMesh(const StaticMesh& _rOther);
	virtual ~SkeletonMesh() = default;
	// Mesh을(를) 통해 상속됨
	virtual void Free() override;
public:
	static SkeletonMesh* Create(LPDIRECT3DDEVICE9 const _pDevice,
		const std::filesystem::path _Path);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;
public:
	HRESULT LoadMeshFromFile(const std::filesystem::path _Path)&;

	/*struct ENGINE_DLL AnimationNotify
	{
		std::string Name{};
		bool bAnimationEnd{ false };
		bool bLoop{ false };
		std::map<double, std::function<void(SkeletonMesh* const)>, std::greater<float>>
			AnimTimeEventCallMapping{};
		std::set<double> HavebeenCalledEvent{};
	};*/
private:
	HRESULT LoadSkeletonMeshImplementation(const aiScene * AiScene,
		const std::filesystem::path _Path);
private:
	Bone* MakeHierarchy(Bone * BoneParent, const aiNode* const AiNode);
private:
	uint32 PrevAnimIndex = 0u;
	uint32 AnimIdx{ 0u };
	uint32 AnimEndAfterAnimIdx{ 0u };

	double CurrentAnimMotionTime{ 0.0 };
	double PrevAnimMotionTime{ 0.0 };
	double TransitionRemainTime = -1.0;
	double TransitionDuration = 0.0;

	uint32 MaxAnimIdx{ 0u };
	bool bBoneDebug = false;
private:
	// AnimNotify CurrentNotify{};
private:
	std::vector<Matrix> RenderBoneMatricies{};
	std::string         RootBoneName{};
	std::shared_ptr<std::set<std::string>> BoneNameSet{};
	double PrevAnimAcceleration = 1.f;
	double Acceleration = 1.f;

	std::vector<AnimationInformation>                          AnimInfoTable{};
	std::unordered_map<std::string, uint32>                     AnimIdxFromName{};

	std::shared_ptr<AnimationTrack>                             _AnimationTrack{};
	std::unordered_map<std::string, uint64>						BoneTableIdxFromName{};
	std::vector<std::shared_ptr<Bone>>							BoneTable{};
	// VTF 기술로 버텍스 쉐이더에서 애니메이션 스키닝을 수행.
	IDirect3DTexture9* BoneAnimMatrixInfo{ nullptr };
	IDirect3DTexture9* PrevBoneAnimMatrixInfo{ nullptr };

	int32 VTFPitch{ 0 };

	std::filesystem::path FilePath{};
	std::filesystem::path FilePureName{};
};
END
#endif // !_SKELETONMESH_H_



