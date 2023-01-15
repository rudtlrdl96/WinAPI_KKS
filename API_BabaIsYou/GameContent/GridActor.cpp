#include "GridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentConst.h"

GameEngineLevel* GridActor::PuzzleLevel = nullptr;
std::vector<GridActor*> GridActor::vecObjectPool;
size_t GridActor::ReturnActorIndex = 0;
int2 GridActor::GridSize = int2::Zero;
float4 GridActor::ActorSize = float4::Zero;



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


void GridActor::LoadData(TEMP_ACTOR_TYPE _Actor)
{
	if (_Actor < TEMP_ACTOR_TYPE::BABA || _Actor >= TEMP_ACTOR_TYPE::COUNT)
	{
		MsgAssert("잘못된 Actor Type이 입력되었습니다.");
		return;
	}

	// Todo : File Save/Load 시스템이 완성된 후 데이터베이스 로드

	if (TEMP_ACTOR_TYPE::BABA == _Actor)
	{
		SetFrame(1);
		SetLength(4);
		ActorType = ACTOR_DEFINE::ACTOR;
		RenderType = ACTOR_RENDER::CHARACTER;
	}

	if (TEMP_ACTOR_TYPE::BABA_TEXT == _Actor)
	{
		SetFrame(0);
		SetLength(1);
		ActorType = ACTOR_DEFINE::SUBJECT_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}

	if (TEMP_ACTOR_TYPE::IS_TEXT == _Actor)
	{
		SetFrame(792);
		SetLength(1);
		ActorType = ACTOR_DEFINE::VERB_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}

	if (TEMP_ACTOR_TYPE::YOU_TEXT == _Actor)
	{
		SetFrame(864);
		SetLength(1);
		ActorType = ACTOR_DEFINE::DEFINE_TEXT;
		RenderType = ACTOR_RENDER::STATIC;
	}
}

void GridActor::SetGrid(const int2& _Pos)
{
	GridPos = _Pos;
	SetPos({GridPos.x * ContentConst::ACTOR_SIZE.x, GridPos.y * ContentConst::ACTOR_SIZE.y});
}
