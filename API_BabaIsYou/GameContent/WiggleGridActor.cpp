#include "WiggleGridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"
#include "WiggleMapToolActor.h"

WiggleGridActor::WiggleGridActor()
{
}

WiggleGridActor::~WiggleGridActor()
{
}

void WiggleGridActor::SetRender(const int2& _Pos, int _ActorEnum, DIR_FLAG _Dir)
{
	if (true == IsOver(_Pos))
	{
		MsgAssert("그리드 범위를 벗어나 접근하려했습니다");
		return;
	}

	vecWiggleActors[_Pos.y][_Pos.x]->SetRender(_ActorEnum, _Dir);
	TileCheck(_Pos);
}

void WiggleGridActor::InitGrid(const int2& _Size)
{
	GridSize = _Size;
	vecWiggleActors.reserve(GridSize.y);

	for (size_t y = 0; y < vecWiggleActors.capacity(); y++)
	{
		vecWiggleActors.push_back(std::vector<WiggleMapToolActor*>());
		vecWiggleActors[y].reserve(GridSize.x);

		for (size_t x = 0; x < vecWiggleActors[y].capacity(); x++)
		{
			vecWiggleActors[y].push_back(GetLevel()->CreateActor<WiggleMapToolActor>());
			vecWiggleActors[y][x]->SetPos(
				{ (ContentConst::ACTOR_SIZE.x * x) + ContentConst::ACTOR_SIZE.half().x,
				(ContentConst::ACTOR_SIZE.y * y) + ContentConst::ACTOR_SIZE.half().y });
			vecWiggleActors[y][x]->SetRender(-1, DIR_FLAG::RIGHT);
			vecWiggleActors[y][x]->Off();
			vecWiggleActors[y][x]->SetGridPos({ x, y });
		}
	}
}

bool WiggleGridActor::IsOver(const int2& _Pos) const
{
	if (0 > _Pos.x || GridSize.x <= _Pos.x ||
		0 > _Pos.y || GridSize.y <= _Pos.y)
	{
		return true;
	}

	return false;
}

int WiggleGridActor::GetActorEnum(const int2& _Pos) const
{
	if (true == IsOver(_Pos))
	{
		MsgAssert("그리드 밖을 참조하려 했습니다");
		return -1;
	}

	return vecWiggleActors[_Pos.y][_Pos.x]->GetIndex();
}

int WiggleGridActor::GetDir(const int2& _Pos) const
{
	return 0;
}

void WiggleGridActor::ActiveGridActors()
{
	for (size_t y = 0; y < GridSize.y; y++)
	{
		for (size_t x = 0; x < GridSize.x; x++)
		{
			if (0 <= vecWiggleActors[y][x]->GetIndex())
			{
				vecWiggleActors[y][x]->On();
			}
		}
	}
	On();
}

void WiggleGridActor::DisableGridActors()
{
	Off();

	for (size_t y = 0; y < GridSize.y; y++)
	{
		for (size_t x = 0; x < GridSize.x; x++)
		{
			vecWiggleActors[y][x]->Off();
		}
	}
}

void WiggleGridActor::ResetGridActors()
{
	for (size_t y = 0; y < vecWiggleActors.size(); y++)
	{
		for (size_t x = 0; x < vecWiggleActors[y].size(); x++)
		{
			vecWiggleActors[y][x]->SetRender(-1, DIR_FLAG::RIGHT);
			vecWiggleActors[y][x]->Off();
		}
	}
}

void WiggleGridActor::TileCheck(const int2& _Pos)
{
	if (true == IsOver(_Pos))
	{
		return;
	}

	if (true == vecWiggleActors[_Pos.y][_Pos.x]->IsTile())
	{
		vecWiggleActors[_Pos.y][_Pos.x]->SetTile(GetNeighborFlagKey(_Pos));
	}

	int2 UpPos = _Pos + int2::Up;
	if (false == IsOver(UpPos) && true == vecWiggleActors[UpPos.y][UpPos.x]->IsTile())
	{
		vecWiggleActors[UpPos.y][UpPos.x]->SetTile(GetNeighborFlagKey(UpPos));
	}

	int2 DownPos = _Pos + int2::Down;
	if (false == IsOver(DownPos) && true == vecWiggleActors[DownPos.y][DownPos.x]->IsTile())
	{
		vecWiggleActors[DownPos.y][DownPos.x]->SetTile(GetNeighborFlagKey(DownPos));
	}

	int2 LeftPos = _Pos + int2::Left;
	if (false == IsOver(LeftPos) && true == vecWiggleActors[LeftPos.y][LeftPos.x]->IsTile())
	{
		vecWiggleActors[LeftPos.y][LeftPos.x]->SetTile(GetNeighborFlagKey(LeftPos));
	}

	int2 RightPos = _Pos + int2::Right;
	if (false == IsOver(RightPos) && true == vecWiggleActors[RightPos.y][RightPos.x]->IsTile())
	{
		vecWiggleActors[RightPos.y][RightPos.x]->SetTile(GetNeighborFlagKey(RightPos));
	}

}

int WiggleGridActor::GetNeighborFlagKey(const int2& _Pos)
{
	int ResultKey = 0;

	int2 UpPos = _Pos + int2::Up;
	if (false == IsOver(UpPos) && true == IsEqulsEnum(_Pos, UpPos))
	{
		ResultKey |= DIR_FLAG::UP;
	}

	int2 DownPos = _Pos + int2::Down;
	if (false == IsOver(DownPos) && true == IsEqulsEnum(_Pos, DownPos))
	{
		ResultKey |= DIR_FLAG::DOWN;
	}

	int2 LeftPos = _Pos + int2::Left;
	if (false == IsOver(LeftPos) && true == IsEqulsEnum(_Pos, LeftPos))
	{
		ResultKey |= DIR_FLAG::LEFT;
	}

	int2 RightPos = _Pos + int2::Right;
	if (false == IsOver(RightPos) && true == IsEqulsEnum(_Pos, RightPos))
	{
		ResultKey |= DIR_FLAG::RIGHT;
	}

	return ResultKey;
}

bool WiggleGridActor::IsEqulsEnum(const int2& _Pos1, const int2& _Pos2)
{
	if (true == IsOver(_Pos1) || true == IsOver(_Pos2))
	{
		return false;
	}

	return vecWiggleActors[_Pos1.y][_Pos1.x]->GetIndex() == vecWiggleActors[_Pos2.y][_Pos2.x]->GetIndex();
}