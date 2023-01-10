#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>

std::vector<std::vector<GridActor::GridData>> GridActor::vecGridData;

GridActor::GridActor()
{
}

GridActor::~GridActor()
{
}

void GridActor::InitGrid(const int2& _Size)
{
	vecGridData.resize(_Size.y);

	for (size_t y = 0; y < vecGridData.size(); y++)
	{
		vecGridData[y].resize(_Size.x);
	}
}

void GridActor::ClearGrid()
{
	for (size_t y = 0; y < vecGridData.size(); y++)
	{
		for (size_t x = 0; x < vecGridData[y].size(); x++)
		{
			vecGridData[y][x].Clear();
		}
	}
}

void GridActor::DeleteGrid()
{
	for (size_t y = 0; y < vecGridData.size(); y++)
	{
		vecGridData[y].clear();
	}

	vecGridData.clear();
}

void GridActor::MoveStart(const int2& _Start, const int2& _Dest)
{

}

void GridActor::MoveEnd(const int2& _Start, const int2& _Dest)
{

}

bool GridActor::TryMove(const int2& _Dir)
{
	if (0 == vecGridData.size())
	{
		MsgAssert("vecTextGrid를 초기화 하지 않고 이동하려 했습니다.");
		return false;
	}
	
	int2 CurPos = GetGridPos();
	int2 NextPos = GetGridPos() + _Dir;
	
	// 그리드 초과
	if (true == IsGridOver(NextPos))
	{
		return false;
	}

	// Stop Check
	if (true == vecGridData[NextPos.y][NextPos.x].IsStop())
	{
		return false;
	}
	
	Move(CurPos, NextPos);

	return true;
}


bool GridActor::CanPush(const int2& _Pos, const int2& _Dir) const
{
	int2 NormalizeDir = int2::Normalize(_Dir);
	int2 CurPos = _Pos;

	//while ()
	//{
	//
	//}

	return true;
}

bool GridActor::IsGridOver(const int2& _Pos) const
{
	if (_Pos.y < 0 || _Pos.y >= vecGridData.size() ||
		_Pos.x < 0 || _Pos.x >= vecGridData[0].size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GridActor::Move(const int2& _CurPos, const int2& _NextPos)
{
	MoveStart(_CurPos, _NextPos);
	SetGridPos(_NextPos);
	MoveEnd(_CurPos, _NextPos);
}