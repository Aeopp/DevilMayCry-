#ifndef _SKELETONMESH_H_
#define _SKELETONMESH_H_
#include "StaticMesh.h"
#include "AnimationBlendInfo.h"
#include "AnimationTrack.h"
#include "Node.h"
#include "AnimationInformation.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <map>
#include <functional>
#include <set>
#include "AnimNotify.h"

class aiNode;
BEGIN(ENGINE)
class ENGINE_DLL SkeletonMesh final : public StaticMesh
{
private:
	explicit SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit SkeletonMesh(const SkeletonMesh& _rOther);
	virtual ~SkeletonMesh() = default;
	// Mesh��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static SkeletonMesh* Create(LPDIRECT3DDEVICE9 const _pDevice,
								const std::filesystem::path _Path,
								const std::any & InitParams);
	// Mesh��(��) ���� ��ӵ�
	virtual Resource* Clone() override;
	virtual void      Editor()override;
	virtual std::string GetName() override;
	void BindVTF(ID3DXEffect * Fx)&;
public:
	bool    IsAnimationEnd();
	void    EnablePrevVTF()&;
	void    DisablePrevVTF()&;
	void    Update(const float DeltaTime)&;
	void    BoneDebugRender(const Matrix & OwnerTransformWorld,ID3DXEffect* const Fx)&;
	void    VTFUpdate()&;
	Node* GetRootNode()&;
	Node* GetNode(const std::string & NodeName)&;
	//      �� ��Ű�� ��Ʈ�������� ToRoot ��Ʈ������ ��� 
	//      (���� ��Ű�� ������Ʈ�� ���� �ʴ´ٸ� ��ȯ���� ������ ��Ű�� �������� ����)
	std::optional<Matrix> GetNodeToRoot(const std::string & NodeName)&;

	void   PlayAnimation(
		const std::string & InitAnimName,
		const bool  bLoop ,
		const AnimNotify & _Notify = {});
	void   PlayAnimation(
		const uint32 AnimationIndex,
		const bool  bLoop,
		const AnimNotify & _Notify = {});

	void    ContinueAnimation()&;
	void    StopAnimation();
	void	AnimationEnd()&;
	// 0 ~ 1 ����ȭ 
	float   PlayingTime();
	//                       ����ȭ�� �ð����� �Ѱ��ּ��� ������ ����� Clamp
	void    SetPlayingTime(float NewTime);
	std::optional<AnimationInformation> GetAnimInfo(const std::string & AnimName) const&;

	void SetDeltaTimeFactor(const float DeltaTimeFactor);
private:
	void	AnimationEditor()&;
	void	NodeEditor();
	void    AnimationUpdateImplementation()&;
	void    AnimationSave(const std::filesystem::path & FullPath)&;
	void    AnimationLoad(const std::filesystem::path & FullPath)&;
private:
	virtual HRESULT LoadMeshImplementation(
		const aiScene * AiScene,
		const std::filesystem::path _Path,
		const std::any & InitParams)override;

	Node* MakeHierarchy(Node * NodeParent, const aiNode* const AiNode ,
		const std::unordered_map<std::string,
		std::pair<uint32, Matrix>>&BoneTableParserInfo);
	// Node* MakeHierarchyForclones(Node* const Parent,const Node* const SpProtoNode);
	void InitTextureForVertexTextureFetch()&;
	void AnimationNotify()&;
private:
	float DeltaTimeFactor = 1.f;
	std::string RootMotionStartName = "root_$AssimpFbx$_Translation";
	bool bRootMotion = true;
	std::string PrevAnimName{};
	std::string AnimName{};
	float  CurrentAnimMotionTime{ 0.0 };
	float  PrevAnimMotionTime{ 0.0 };
	float  TransitionRemainTime = -1.0;
	float  TransitionDuration = 0.0;
	bool   bLoop = false;
	bool   bAnimationEnd = true;
	bool   bAnimStop = false;
	bool   bAnimSaveButton = false;

	AnimNotify           CurAnimNotify{};
	AnimationInformation CurPlayAnimInfo{};
	AnimationInformation PrevPlayAnimInfo{};
	IDirect3DTexture9* BoneAnimMatrixInfo{ nullptr };
	IDirect3DTexture9* PrevBoneAnimMatrixInfo{ nullptr };
	int32 VTFPitch{ 0 };
	std::vector<Matrix> BoneSkinningMatries{};
	std::vector<Matrix> PrevBoneSkinningMatries{};
	bool bHasAnimation = false;
	std::string RootNodeName{};
	std::shared_ptr<std::map<uint32, std::string>>				AnimIndexNameMap{};
	std::shared_ptr<std::map<std::string,AnimationInformation>> AnimInfoTable{};
	std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<Node>>> Nodes{};
};
END
#endif // !_SKELETONMESH_H_



