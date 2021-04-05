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
<<<<<<< HEAD

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
=======
public:
	void    EnablePrevVTF()&;
	void    DisablePrevVTF()&;
	void    Update(const float DeltaTime)&;
	void    VTFUpdate()&;
	Node*   GetRootNode()&;
	Node*	GetNode(const std::string & NodeName)&;
	void    PlayAnimation(
		const std::string & InitAnimName, 
		const bool  bLoop ,
		const AnimNotify & _Notify = {});
	void    ContinueAnimation()&;
	void    StopAnimation();
	void	AnimationEnd()&;
	// 0 ~ 1 정규화 
	float   PlayingTime();
	//                       정규화된 시간으로 넘겨주세요 범위를 벗어나면 Clamp
	void    SetPlayingTime(float NewTime);
	std::optional<AnimationInformation> GetAnimInfo(const std::string& AnimName) const&;
private:
	void	AnimationEditor()&;
	void	NodeEditor();
	void    AnimationUpdateImplementation()&;
	void    AnimationSave(const std::filesystem::path & FullPath)&;
	void    AnimationLoad(const std::filesystem::path & FullPath)&;
private:
	HRESULT LoadSkeletonMeshImplementation(const aiScene * AiScene,
								const std::filesystem::path _Path);
	Node*  MakeHierarchy(Node * NodeParent, const aiNode* const AiNode ,
		const std::unordered_map<std::string,
		std::pair<uint32, Matrix>>&BoneTableParserInfo);
	// Node* MakeHierarchyForclones(Node* const Parent,const Node* const SpProtoNode);
	void InitTextureForVertexTextureFetch()&;
	void AnimationNotify()&;
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
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
<<<<<<< HEAD

	std::filesystem::path FilePath{};
	std::filesystem::path FilePureName{};
=======
	std::vector<Matrix> BoneSkinningMatries{};
	std::vector<Matrix> PrevBoneSkinningMatries{};
	bool bHasAnimation = false;
	std::string RootNodeName{};
	std::shared_ptr<std::map<std::string,AnimationInformation>> AnimInfoTable{};
	std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<Node>>> Nodes{};
>>>>>>> a8ab4edb3eac470353904547d5f18b753d795ff0
};
END
#endif // !_SKELETONMESH_H_



