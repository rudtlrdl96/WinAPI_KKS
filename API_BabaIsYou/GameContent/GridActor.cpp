#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentConst.h"

std::vector<std::vector<GridActor::GridData>> GridActor::vecGridData;
float4 GridActor::ActorSize = float4::Zero;

GridActor::GridActor()
{
}

GridActor::~GridActor()
{
}

void GridActor::Start()
{
	//InitRender();
}

void GridActor::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (GetGridPos() != GetPrevPos())
	{
		MoveProgress += _DT * ContentConst::MOVE_SPEED;

		SetPos(Lerp(GetScreenPos(GetPrevPos()), GetScreenPos(GetGridPos()), MoveProgress));

		if (MoveProgress >= 1.0f)
		{
			SetPos(GetScreenPos(GetGridPos()));
			MoveProgress = 0.0f;
			SetPrevPos(GetPrevPos());

			Move(GetGridPos() + int2{1, 0});
		}
	}
}

void GridActor::InitGrid(const int2& _GridSize, const float4& _ActorSize)
{
	vecGridData.resize(_GridSize.y);

	for (size_t y = 0; y < vecGridData.size(); y++)
	{
		vecGridData[y].resize(_GridSize.x);
	}

	ActorSize = _ActorSize;
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

float4 GridActor::GetScreenPos(const int2& _Pos)
{
	float4 ScreenPos;

	ScreenPos.x = ActorSize.x * _Pos.x;
	ScreenPos.y = ActorSize.y * _Pos.y;

	return ScreenPos;
}

bool GridActor::TryMove(const int2& _Dir)
{
	if (0 == vecGridData.size())
	{
		MsgAssert("vecTextGrid를 초기화 하지 않고 이동하려 했습니다.");
		return false;
	}
	
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
	
	Move(NextPos);

	return true;
}


bool GridActor::CanPush(const int2& _Pos, const int2& _Dir) const
{
	int2 NormalizeDir = int2::Normalize(_Dir);
	int2 CurPos = _Pos;
	

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

void GridActor::Move(const int2& _NextPos)
{
	SetPrevPos(GetGridPos());
	SetGridPos(_NextPos);
}