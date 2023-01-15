#pragma once
#include <vector>
#include <map>
#include <string>
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentMath.h"
#include "ContentConst.h"
#include "WiggleActor.h"

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

class GameEngineLevel;
class GridActor : public WiggleActor
{
public:
	GridActor();
	~GridActor();

	void Start() override;

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	static GridActor* GetActor(TEMP_ACTOR_TYPE _Type);

	static void InitGridActor(GameEngineLevel* _PuzzleLevel, const int2& _GridSize, const float4& _ActorSize);
	static void ClearGridActor();
	static void DeleteGridActor();

	void LoadData(TEMP_ACTOR_TYPE _Actor);
	void SetGrid(const int2& _Pos);

protected:

private:
	static GameEngineLevel* PuzzleLevel;
	static std::vector<GridActor*> vecObjectPool;
	static size_t ReturnActorIndex;
	static int2 GridSize;
	static float4 ActorSize;
	
	ACTOR_DEFINE ActorType = ACTOR_DEFINE::ACTOR;
	ACTOR_RENDER RenderType = ACTOR_RENDER::STATIC;

	int2 GridPos = int2::Zero;
};