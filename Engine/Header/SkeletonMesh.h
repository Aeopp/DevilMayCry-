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
	// Mesh을(를) 통해 상속됨
	virtual void Free() override;
public:
	static SkeletonMesh* Create(LPDIRECT3DDEVICE9 const _pDevice,
								const std::filesystem::path _Path,
								const std::any & InitParams);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;
	virtual void      Editor()override;
	virtual std::string GetName() override;
	void BindVTF(ID3DXEffect * Fx)&;
public:
	bool    IsAnimationEnd();
	void    EnablePrevVTF()&;
	void    DisablePrevVTF()&;
	std::tuple<Vector3, Quaternion, Vector3>    Update(const float DeltaTime)&;
	void    BoneDebugRender(const Matrix & OwnerTransformWorld,ID3DXEffect* const Fx)&;
	void    VTFUpdate()&;
	Node*   GetRootNode()&;
	Node*   GetNode(const std::string & NodeName)&;
	//      본 스키닝 매트릭스에서 ToRoot 매트릭스를 계산 
	//      (현재 스키닝 업데이트를 하지 않는다면 반환값은 마지막 스키닝 했을시의 정보)
	std::optional<Matrix> GetNodeToRoot(const std::string & NodeName)&;

	void   PlayAnimation(
		const std::string & InitAnimName,
		const bool  bLoop,
		const AnimNotify & _Notify = {} ,
		const float _CurrentAccelerationFactor = 1.0f, 
		const float _CurrentTransitionTimeFactor = 1.0f);
	void   PlayAnimation(
		const uint32 AnimationIndex,
		const bool  bLoop,
		const AnimNotify & _Notify = {} ,
		const float _CurrentAccelerationFactor = 1.0f,
		const float _CurrentTransitionTimeFactor = 1.0f);

	void    ContinueAnimation()&;
	void    StopAnimation();
	void	AnimationEnd()&;
	// 0 ~ 1 정규화 
	float   PlayingTime();
	//  정규화된 시간으로 넘겨주세요 범위를 벗어나면 Clamp
	void    SetPlayingTime(float NewTime);
	std::optional<AnimationInformation> GetAnimInfo(const std::string & AnimName) const&;

	void SetDeltaTimeFactor(const float DeltaTimeFactor);
private:
	void	AnimationEditor()&;
	void	NodeEditor();
	std::tuple<Vector3, Quaternion, Vector3>    AnimationUpdateImplementation()&;
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

	static inline std::string NormallyRootMotionTransitionName = 
		"root_$AssimpFbx$_Translation";
	static inline std::string NormallyRootMotionScaleName =
		"root_$AssimpFbx$_Scaling";
	static inline std::string NormallyRootMotionRotationName =
		"root_$AssimpFbx$_Rotation";
private:
	std::string RootMotionScaleName = NormallyRootMotionScaleName;
	std::string RootMotionRotationName = NormallyRootMotionRotationName;
	std::string RootMotionTransitionName = NormallyRootMotionTransitionName;

	Vector3 CalcRootMotionDeltaPos(std::optional<float> bTimeBeyondAnimation,
									const std::string & _TargetAnimName,
									const float AnimDuraion,
									const float AnimPrevFrameMotionTime,
									const float AnimMotionTime)&;

	Vector3 CalcRootMotionDeltaScale(std::optional<float> bTimeBeyondAnimation,
		const std::string & _TargetAnimName,
		const float AnimDuraion,
		const float AnimPrevFrameMotionTime,
		const float AnimMotionTime)&;

	// 아직 회전은 테스트 하지 않았음.
	Quaternion CalcRootMotionDeltaQuat(std::optional<float> bTimeBeyondAnimation,
		const std::string & _TargetAnimName,
		const float AnimDuraion,
		const float AnimPrevFrameMotionTime,
		const float AnimMotionTime)&;

public:
	// 노드 정보는 클론들끼리 공유하므로 하나의 클론이 설정한 값으로 모든 클론이 작동.
	void    EnableScaleRootMotion(const std::string & ScalingRootName = "");
	void    EnableRotationRootMotion(const std::string & RotationRootName = "");
	void    EnableTransitionRootMotion(const std::string & TransitionRootName = "");
	void    DisableScaleRootMotion();
	void    DisableRotationRootMotion();
	void    DisableTransitionRootMotion();

	float DeltaTimeFactor = 1.f;
	bool  bRootMotionScale = false;
	bool  bRootMotionRotation = false;
	bool  bRootMotionTransition = false;

	std::string PrevAnimName{};
	std::string AnimName{};
	
	float  CurrentAccelerationFactor = 1.0f;
	float  CurrentTransitionTimeFactor = 1.0f;
	float  CurrentAnimMotionTime{ 0.0 };
	float  CurrentAnimPrevFrameMotionTime{ 0.0f };

	float  PrevAnimMotionTime     { 0.0f };
	float  PrevAnimPrevFrameMotionTime{ 0.0f };
	float  PrevAccelerationFactor{ 1.0f };

	float  TransitionRemainTime = -1.0;
	float  TransitionDuration = 0.0;
	bool   bLoop = false;
	bool   bAnimationEnd = true;
	bool   bAnimStop = false;
	bool   bAnimSaveButton = false;

	Vector3    RootMotionLastCalcDeltaScale = { 0,0,0 };
	Quaternion RootMotionLastCalcDeltaQuat = { 0,0,0,1 };
	Vector3    RootMotionLastCalcDeltaPos = { 0,0,0 };

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



