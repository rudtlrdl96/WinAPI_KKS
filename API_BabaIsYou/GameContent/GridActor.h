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

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	const int2 GetGridPos() const
	{
		return GridPos;
	}

	void SetGridPos(const int2& _Pos)
	{
		GridPos.x = _Pos.x;
		GridPos.y = _Pos.y;
	}

	bool IsStop() const
	{
		return false;
	}

	bool IsPush() const
	{
		return true;
	}

	bool CanPush(const int2& _Pos, const int2& _Dir) const;
	bool IsGridOver(const int2& _Pos) const;
	bool TryMove(const int2& _Dir);

protected:
	static std::vector<std::vector<GridData>> vecGridData;

	static void InitGrid(const int2& _Size);
	static void ClearGrid();
	static void DeleteGrid();

	virtual void MoveStart(const int2& _Start, const int2& _Dest);
	virtual void MoveEnd(const int2& _Start, const int2& _Dest);

private:
	int2 GridPos = {0, 0};

	void Move(const int2& _CurPos, const int2& _NextPos);
};