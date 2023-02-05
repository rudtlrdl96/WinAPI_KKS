#pragma once
#include <vector>
#include <map>
#include <string>
#include "ContentMath.h"
#include "ContentEnum.h"
#include "WiggleActor.h"

class GameEngineLevel;
class Rule;
class PuzzleActor : public WiggleActor
{
	friend Rule;
private:
	class GridData
	{
	public:
		std::map<int, PuzzleActor*> mapDatas;

		void push_back(PuzzleActor* _Actor);
		void erase(PuzzleActor* _Actor);
		void clear();
		void Push(const int2& _Pos, const int2& _Dir, bool _IsInputMove);
		void DeathCheck();
		bool equals(const std::string_view& _Name);
		size_t GetDefine();
		void SinkCheckReset();
		void Sink();
	private:
		bool IsSinkValue = false;
	};

	class BehavoirData
	{
	public:
		BEHAVIOR Behavior = BEHAVIOR::WAIT;
		int Value = -1;
	};

#pragma region Static

public:
	static void InitGridActor(GameEngineLevel* _PuzzleLevel);
	static void SetGridLength(const int2& _Length);
	static void ResetPuzzleActor();
	static void DeletePuzzleActor();

	static float4 GetScreenPos(const int2& _GridPos);

	static void AllActorUndo();
	static void AllActorRuleCheck();
	static void PuzzleActorDeathCheck();
	static void PuzzleActorSaveBehavior();
	static void MoveAllYouBehavior(const int2& _Dir);
	static void MoveAllMoveBehavior();

	static PuzzleActor* CreatePuzzlActor(int _Type);

	static PuzzleActor* GetTextActor(const int2& _Pos);
	static std::map<int, PuzzleActor*>& GetActors(int _ActorIndex);
	static std::map<int, PuzzleActor*>& GetActors(ACTOR_DEFINE _Define);

	static bool IsAnyMove();
	static void AnyMoveCheckReset();
	static bool IsOver(const int2& _GridPos);
	static bool IsWin();

private:
	static GameEngineLevel* PuzzleLevel;
	static size_t ObjectPoolCount;
	static bool AnyActorMoveCheck;
	static bool WinCheckValue;

	// 액터마다 고유 키를 주기위한 변수 절대 임의로 값 수정 x
	static int NextActorKey;
	static int2 GridLength;

	static std::vector<std::vector<GridData>> vecGridDatas;
	static std::vector<std::vector<PuzzleActor*>> vecTextDatas;

	// ObjectPooling 
	static std::vector<PuzzleActor*> vecObjectPool;

	// 액터 타입별 데이터 정렬
	static std::map<int, std::map<int, PuzzleActor*>> mapActorDatas;
	// 액터 Deinfe 데이터 정렬
	static std::map<ACTOR_DEFINE, std::map<int, PuzzleActor*>> mapDefineActorDatas;


#pragma endregion


public:
	PuzzleActor();
	~PuzzleActor();

	PuzzleActor(const PuzzleActor& _Other) = delete;
	PuzzleActor(PuzzleActor&& _Other) noexcept = delete;
	PuzzleActor& operator=(const PuzzleActor& _Other) = delete;
	PuzzleActor& operator=(PuzzleActor&& _Other) noexcept = delete;

	void LoadData(int _Actor, int2 _Dir, bool _IsInit);
	void LoadData(int _Actor, DIR_FLAG _Dir, bool _IsInit);
	void RuleCheck();
	void ResetValues();
	void SetGrid(const int2& _Pos);
	void AddDefine(ACTOR_DEFINE _Info);
	void RemoveDefine(ACTOR_DEFINE _Info);
	bool IsDefine(ACTOR_DEFINE _Info);
	void SaveBehaviorInfo();
	int GetArrowEnum() const;
	ACTOR_DEFINE GetArrowDefine() const;

	ACTOR_TYPE GetActorType() const;
	int2 GetGridPos() const;
	int2 GetMoveDir() const;

protected:
	void Start() override;
	void Update(float _DT) override;

private:
	std::string ActorName = "";

	int ActorEnum = -1;
	ACTOR_TYPE ActorType = ACTOR_TYPE::ACTOR;
	ACTOR_RENDER_TYPE RenderType = ACTOR_RENDER_TYPE::STATIC;

	int ArrowEnum = -1;
	ACTOR_DEFINE ArrowDefine = ACTOR_DEFINE::NONE;

	std::map<int, Rule*> mapRules;
	std::vector<BehavoirData> CurFramesBehaviorBuffer;
	std::vector<std::vector<BehavoirData>> vecBehaviorBuffer;

	const int ActorKey = 0;

	float ParticleTime = 0.0f;
	float NextParticleTime = 0.0f;

	int2 MoveDir = int2::Right;
	int2 PrevPos = {-1, -1};
	int2 GridPos = int2::Zero;

	size_t DefineData = static_cast<size_t>(ACTOR_DEFINE::NONE);

	PARTICLE_COLOR ActorColor = PARTICLE_COLOR::WHITE;

	bool IsDeath = false;
	bool IsMove = false;
	float MoveProgress = 0.0f;

	inline size_t GetDefine()
	{
		return DefineData;
	}

	void Undo();

	bool Move(bool _IsInputMove);
	void UndoMove();
	void Push();
	void UndoPush();

	void TurnLeft();
	void UndoTurnLeft();
	void TurnRight();
	void UndoTurnRight();

	void SetDefine(size_t _Info);
	void SetDefine(ACTOR_DEFINE _Info);
	void UndoAddDefine(ACTOR_DEFINE _Info);
	void UndoRemoveDefine(ACTOR_DEFINE _Info);

	void SetDir(const int2& _Dir);
	
	void ActorDeath(ACTOR_DEFINE _DeathCause);
	void UndoActorDeath();

	void AllPushDir(const int2& _Dir, bool _IsInputMove);
	bool CanMove(const int2& _NextPos);
	void WinCheck();
	void AddRule();
	void RemoveRule();

	void SetTileRender();
	void ActorParticleCreate(float _DT);
};