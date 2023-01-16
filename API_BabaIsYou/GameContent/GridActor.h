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

enum class DEFINE_INFO
{
	YOU = 1 << 0,
	PUSH = 1 << 1,
	STOP = 1 << 2,
};

class GameEngineLevel;
class GridActor : public WiggleActor
{
private:
	class GridData
	{
	public:
		std::vector<GridActor*> vecDatas;

		void push_back(GridActor* _Actor)
		{
			vecDatas.push_back(_Actor);
		}

		void clear()
		{
			vecDatas.clear();
		}

		bool IsStop()
		{
			for (GridActor* Data : vecDatas)
			{
				if (true == Data->IsDefine(DEFINE_INFO::STOP))
				{
					return true;
				}
			}

			return false;
		}

		void Push(const int2& _Dir)
		{
			for (GridActor* Data : vecDatas)
			{
				if (true == Data->IsDefine(DEFINE_INFO::PUSH))
				{
					Data->Push(_Dir);
				}
			}
		}

		bool IsPush()
		{
			for (GridActor* Data : vecDatas)
			{
				if (true == Data->IsDefine(DEFINE_INFO::PUSH))
				{
					return true;
				}
			}
			return false;
		}
	};

public:
	static void InitGridActor(GameEngineLevel* _PuzzleLevel, const int2& _GridSize, const float4& _ActorSize);
	static void ResetGridActor();
	static void ClearGrid();
	static void DeleteGridActor();

	static GridActor* GetActor(TEMP_ACTOR_TYPE _Type);	
	static float4 GetScreenPos(const int2& _GridPos);
	static bool IsOver(const int2& _GridPos);

private:
	static GameEngineLevel* PuzzleLevel;
	static std::vector<GridActor*> vecObjectPool;
	static size_t ReturnActorIndex;
	static int2 GridSize;
	static float4 ActorSize;

	static std::vector<std::vector<GridData>> vecGridDatas;

public:
	GridActor();
	~GridActor();

	void Start() override;
	void Update(float _DT) override;
	void LateUpdate(float _DT) override;

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	void LoadData(TEMP_ACTOR_TYPE _Actor);
	void SetGrid(const int2& _Pos);
	void AddDefine(DEFINE_INFO _Info);
	void RemoveDefine(DEFINE_INFO _Info);
	bool IsDefine(DEFINE_INFO _Info);

protected:

private:	
	ACTOR_DEFINE ActorType = ACTOR_DEFINE::ACTOR;
	ACTOR_RENDER RenderType = ACTOR_RENDER::STATIC;

	int2 PrevPos = int2::Zero;
	int2 GridPos = int2::Zero;

	size_t DefineData = 0;

	bool IsMove = false;
	float MoveProgress = 0.0f;

	bool Move(const int2& _NextPos);
	void PushDir(const int2& _Dir);
	void Push(const int2& _Dir);
	bool CanMove(const int2& _NextPos);
};