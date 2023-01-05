#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineLevel::GameEngineLevel()
{
}

GameEngineLevel::~GameEngineLevel()
{
	// 편하게 for문을 돌리게 하기 위해서 17인부터 
	for (GameEngineActor* Actor : Actors)
	{
		// Actors.erase()
		if (nullptr != Actor)
		{
			delete Actor;
			Actor = nullptr;
		}
	}

	Actors.clear();
}

void GameEngineLevel::ActorStart(GameEngineActor* _Actor)
{
	if (nullptr == _Actor)
	{
		MsgAssert("nullptr 액터를 Start하려고 했습니다.");
		return;
	}

	_Actor->Start();
}

void GameEngineLevel::ActorsUpdate()
{
	std::list<GameEngineActor*>::iterator StartIter = Actors.begin();
	std::list<GameEngineActor*>::iterator EndIter = Actors.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->Update();
	}
}

void GameEngineLevel::ActorsRender()
{
	std::list<GameEngineActor*>::iterator StartIter = Actors.begin();
	std::list<GameEngineActor*>::iterator EndIter = Actors.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->Render();
	}
}