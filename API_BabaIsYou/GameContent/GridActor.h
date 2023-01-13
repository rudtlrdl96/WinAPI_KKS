#pragma once
#include <vector>
#include <string>
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentMath.h"
#include "WiggleActor.h"

class GridActor : public WiggleActor
{
private:
	class GridData
	{
	public:
		GridData()
		{
			vecDatas.reserve(16);
		}

		bool IsStop() const
		{
			for (GridActor* _Node : vecDatas)
			{
				if (true == _Node->IsStop())
				{
					return true;
				}
			}

			return false;
		}

		bool IsPush() const
		{
			for (GridActor* _Node : vecDatas)
			{
				if (true == _Node->IsPush())
				{
					return true;
				}
			}

			return false;
		}

		void AddActorData(GridActor* _ActorData)
		{
			if (nullptr == _ActorData)
			{
				MsgAssert("nullptr Actor를 추가하려 했습니다");
			}

			vecDatas.push_back(_ActorData);
		}

		void Clear()
		{
			vecDatas.clear();
		}

	private:
		std::vector<GridActor*> vecDatas;
	};

public:
	GridActor();
	~GridActor();

	void Start() override;
	void Update(float _DT) override;

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	const int2 GetGridPos() const
	{
		return GridPos;
	}

	const int2 GetPrevPos() const
	{
		return PrevPos;
	}

	void SetPrevPos(const int2& _Pos)
	{
		PrevPos = _Pos;
	}

	void SetGridPos(const int2& _Pos)
	{
		GridPos = _Pos;
	}

	bool IsStop() const
	{
		return false;
	}

	bool IsPush() const
	{
		return true;
	}

	static void InitGrid(const int2& _GridSize, const float4& _ActorSize);
	static void ClearGrid();
	static void DeleteGrid();

	static float4 GetScreenPos(const int2& _Pos);

	bool CanPush(const int2& _Pos, const int2& _Dir) const;
	bool IsGridOver(const int2& _Pos) const;
	bool TryMove(const int2& _Dir);

protected:

private:
	static std::vector<std::vector<GridData>> vecGridData;
	static float4 ActorSize;

	int2 PrevPos = {0, 0};
	int2 GridPos = {0, 0};

	float MoveProgress = 0.0f;

	void Move(const int2& _NextPos);
};