#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
std::vector<GridActor*> GridActor::vecObjectPool;
size_t GridActor::ReturnActorIndex = 0;
int2 GridActor::GridSize = int2::Zero;
float4 GridActor::ActorSize = float4::Zero;

#pragma region StaticFunc

GridActor* GridActor::GetActor(TEMP_ACTOR_TYPE _Type)
{
	if (ReturnActorIndex >= vecObjectPool.size())
	{
		MsgAssert("Object Pool 사이즈를 초과했습니다.");
		return nullptr;
	}

	if (nullptr == vecObjectPool[ReturnActorIndex])
	{
		MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
		return nullptr;
	}

	vecObjectPool[ReturnActorIndex]->On();
	return vecObjectPool[ReturnActorIndex++];
}

void GridActor::InitGridActor(GameEngineLevel* _PuzzleLevel, const int2& _GridSize, const float4& _ActorSize)
{
	if (nullptr != PuzzleLevel)
	{
		MsgAssert("InitGridActor를 중복 호출 하였습니다");
		return;
	}

	if (nullptr == _PuzzleLevel)
	{
		MsgAssert("nullptr PuzzleLevel이 인자로 입력되었습니다.");
		return;
	}

	PuzzleLevel = _PuzzleLevel;
	GridSize = _GridSize;
	ActorSize = _ActorSize;
	vecObjectPool.reserve(GridSize.x * GridSize.y);

	for (size_t i = 0; i < vecObjectPool.capacity(); i++)
	{
		PuzzleLevel->CreateActor<GridActor>();
	}
}

void GridActor::ClearGridActor()
{
	for (size_t i = 0; i < vecObjectPool.size(); i++)
	{
		if (nullptr == vecObjectPool[i])
		{
			MsgAssert("vecObjectPool 벡터에 nullptr Actor가 존재합니다.");
			return;
		}

		vecObjectPool[i]->Off();
	}

	ReturnActorIndex = 0;
}

void GridActor::DeleteGridActor()
{
	vecObjectPool.clear();
	ReturnActorIndex = 0;
}

float4 GridActor::GetScreenPos(const int2& _GridPos)
{
	return {
		(ContentConst::ACTOR_SIZE.x * _GridPos.x) + ActorSize.half().x,
		(ContentConst::ACTOR_SIZE.y * _GridPos.y) + ActorSize.half().y};
}


bool GridActor::IsOver(const int2& _GridPos)
{
	if (_GridPos.x < 0 ||
		_GridPos.y < 0 ||
		_GridPos.x >= GridSize.x ||
		_GridPos.y >= GridSize.y)
	{
		return true;
	}

	return false;
}

#pragma endregion

GridActor::GridActor()
{
}

GridActor::~GridActor()
{
}

void GridActor::Start()
{
	InitRender("actor.BMP", float4::Zero, ContentConst::ACTOR_SIZE, 0, 4, 10, 24);
	vecObjectPool.push_back(this);
	Off();
}

void GridActor::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (DefineData & static_cast<size_t>(DEFINE_INFO::YOU))
	{
		int2 MoveDir = int2::Zero;

		if (GameEngineInput::IsDown("ArrowUp"))
		{
			MoveDir = int2::Up;
		}
		if (GameEngineInput::IsDown("ArrowDown"))
		{
			MoveDir = int2::Down;
		}
		if (GameEngineInput::IsDown("ArrowLeft"))
		{
			MoveDir = int2::Left;
		}
		if (GameEngineInput::IsDown("ArrowRight"))
		{
			MoveDir = int2::Right;
		}

		if (int2::Zero != MoveDir)
		{
			if (true == Move(GridPos + MoveDir))
			{
				NextAnim();
				SetAnimDir(MoveDir);
			}
		}
	}

	if (true == IsMove)
	{
		MoveProgress += _DT * ContentConst::MOVE_SPEED;

		if (1.0f <= MoveProgress)
		{
			IsMove = false;
			MoveProgress = 1.0f;
		}

		SetPos(Lerp(GetScreenPos(PrevPos), GetScreenPos(GridPos), MoveProgress));
	}
}

void GridActor::LoadData(TEMP_ACTOR_TYPE _Actor)
{
	if (_Actor < TEMP_ACTOR_TYPE::BABA || _Actor >= TEMP_ACTOR_TYPE::COUNT)
	{
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	// Todo : File Save/Load 시스템이 완성된 후 데이터베이스 로드

	// 속성 값 초기화
	DefineData = 0;

	if (TEMP_ACTOR_TYPE::BABA == _Actor)
	{
		SetFrame(1);
		SetLength(4);
		SetDirInterval(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;
		
		// Todo : 테스트용 임시 호출 추후 데이터시스템이 생성되면 삭제
		AddDefine(DEFINE_INFO::YOU);
	}

	if (TEMP_ACTOR_TYPE::BABA_TEXT == _Actor)
	{
		SetFrame(0);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}

	if (TEMP_ACTOR_TYPE::IS_TEXT == _Actor)
	{
		SetFrame(792);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::VERB_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}

	if (TEMP_ACTOR_TYPE::YOU_TEXT == _Actor)
	{
		SetFrame(864);
		SetLength(1);
		SetDirInterval(0);
		ActorType = ACTOR_DEFINE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}

	SetAnimDir(int2::Right);
}

void GridActor::SetGrid(const int2& _Pos)
{
	GridPos = _Pos;
	SetPos(GetScreenPos(GridPos));
}

void GridActor::AddDefine(DEFINE_INFO _Info)
{
	DefineData |= static_cast<size_t>(_Info);
}

void GridActor::RemoveDefine(DEFINE_INFO _Info)
{
	DefineData &= ~static_cast<size_t>(_Info);
}


bool GridActor::Move(const int2& _NextPos)
{
	if (true == IsMove)
	{
		return false;
	}

	if (false == CanMove(_NextPos))
	{
		return false;
	}

	IsMove = true;
	PrevPos = GridPos;
	GridPos = _NextPos;
	MoveProgress = 0.0f;

	return true;
}


bool GridActor::CanMove(const int2& _NextPos)
{
	if (IsOver(_NextPos))
	{
		return false;
	}

	return true;
}