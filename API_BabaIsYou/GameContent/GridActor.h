#pragma once
#include <vector>
#include <map>
#include <string>
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentMath.h"
#include "ContentEnum.h"
#include "WiggleActor.h"

class GameEngineLevel;
class GridActor : public WiggleActor
{
	friend class ActorManager;
public:
#pragma region Enum
	enum class ACTOR_RENDER
	{
		STATIC,
		DYNAMIC,
		CHARACTER,
		TILE,
		BELT,
	};

	enum class ACTOR_DEFINE
	{
		ACTOR,
		SUBJECT_TEXT,
		VERB_TEXT,
		DEFINE_TEXT,
	};

	enum class DEFINE_INFO
	{
		NONE   = 0,
		YOU    = 1 << 0,
		PUSH   = 1 << 1,
		STOP   = 1 << 2,
		MOVE   = 1 << 3,
		HOT	   = 1 << 4,
		MELT   = 1 << 5,
		SINK   = 1 << 6,
		DEFEAT = 1 << 7,
	};

#pragma endregion

private:
	class GridData
	{
	public:
		std::map<int, GridActor*> mapDatas;
		void push_back(GridActor* _Actor);
		void erase(GridActor* _Actor);
		void clear();
		void Push(const int2& _Pos, const int2& _Dir, bool _IsInputMove);
		void DeathCheck();
		size_t GetDefine();
	};

#pragma region Static

public:
	static void InitGridActor(GameEngineLevel* _PuzzleLevel);
	static void ResetGridActor();
	static void ClearGrid();
	static void DeleteGridActor();

	static void MoveAllYouBehavior(const int2& _Dir);
	static void MoveAllMoveBehavior();

	static GridActor* GetActor(TEMP_ACTOR_TYPE _Type);
	static float4 GetScreenPos(const int2& _GridPos);
	static bool IsOver(const int2& _GridPos);

private:
	static GameEngineLevel* PuzzleLevel;
	static size_t ObjectPoolCount;
	static bool AnyActorMoveCheck;

	static int NextActorKey;

	static std::vector<std::vector<GridData>> vecGridDatas;
	static std::vector<GridActor*> vecObjectPool;	
	static std::map<DEFINE_INFO, std::map<int, GridActor*>> mapDefineActorDatas;

#pragma endregion


public:
	GridActor();
	~GridActor();

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	void LoadData(TEMP_ACTOR_TYPE _Actor);
	void SetGrid(const int2& _Pos);
	void AddDefine(DEFINE_INFO _Info);
	void RemoveDefine(DEFINE_INFO _Info);
	bool IsDefine(DEFINE_INFO _Info);
	
	void SaveBehaviorInfo();


protected:
	void Start() override;
	void Update(float _DT) override;

private:	
	ACTOR_DEFINE ActorType = ACTOR_DEFINE::ACTOR;
	ACTOR_RENDER RenderType = ACTOR_RENDER::STATIC;

	std::vector<BEHAVIOR> CurFramesBehaviors;
	std::vector<std::vector<BEHAVIOR>> vecBehaviors;

	const int ActorKey = 0;

	int2 MoveDir = int2::Right;
	int2 PrevPos = {-1, -1};
	int2 GridPos = int2::Zero;

	size_t DefineData = static_cast<size_t>(DEFINE_INFO::NONE);

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

	void SetDir(const int2& _Dir);
	
	void ActorDeath();
	void UndoActorDeath();

	void AllPushDir(const int2& _Dir, bool _IsInputMove);
	bool CanMove(const int2& _NextPos);
};