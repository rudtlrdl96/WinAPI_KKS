#include "WorldMapLevel.h"

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>
#include "MapBackgroundUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"
#include "GrayBackUI.h"
#include "WorldMapActor.h"
#include "PuzzleLevel.h"
#include "WorldMapSelect.h"

WorldMapLevel::WorldMapLevel()
{
}

WorldMapLevel::~WorldMapLevel()
{
}

void WorldMapLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("WorldMap");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMap.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMapActorBack.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMapSelect.BMP"))->Cut(1, 3);

	WorldMapFadeActor = CreateActor<FadeUI>();
	CreateActor<MapBackgroundUI>();
	CreateActor<GrayBackUI>();

	WorldMapSelectActor = CreateActor<WorldMapSelect>();

	if (false == GameEngineInput::IsKey("LevelChange"))
	{
		GameEngineInput::CreateKey("LevelChangeSpace", VK_SPACE);
		GameEngineInput::CreateKey("LevelChangeEnter", VK_RETURN);
	}

	MapSize = { 4, 5 };
	vecWorldMapDatas.reserve(MapSize.y);

	for (int y = 0; y < MapSize.y; y++)
	{
		vecWorldMapDatas.push_back(std::vector<WorldMapActor*>());
		vecWorldMapDatas[y].reserve(MapSize.x);

		for (int x = 0; x < MapSize.x; x++)
		{
			vecWorldMapDatas[y].push_back(CreateActor<WorldMapActor>());
			vecWorldMapDatas[y][x]->SetPos({ 410.0f + (x * 32), 560.0f  - (y * 32)});
		}
	}

	vecWorldMapDatas[0][0]->InitWorldMapActor("Stage_0_0", "Test", 0);
	vecWorldMapDatas[2][1]->InitWorldMapActor("Stage_0_1", "Test", 0);
	vecWorldMapDatas[3][1]->InitWorldMapActor("Stage_0_2", "Test", 0);
	vecWorldMapDatas[2][2]->InitWorldMapActor("Stage_0_3", "Test", 0);
	vecWorldMapDatas[3][2]->InitWorldMapActor("Stage_0_4", "Test", 0);
	vecWorldMapDatas[4][1]->InitWorldMapActor("Stage_0_5", "Test", 0);
	vecWorldMapDatas[3][3]->InitWorldMapActor("Stage_0_6", "Test", 0);
	vecWorldMapDatas[4][2]->InitWorldMapActor("Stage_0_7", "Test", 0);

	WorldMapSelectActor->SetPos(vecWorldMapDatas[0][0]->GetPos());
}

void WorldMapLevel::Update(float _DT)
{
	if (true == WorldMapFadeActor->IsProgress())
	{
		return;
	}

	if (true == GameEngineInput::IsDown("ArrowUp"))
	{
		MoveWorldMap(int2::Down);
	}
	else if (true == GameEngineInput::IsDown("ArrowDown"))
	{
		MoveWorldMap(int2::Up);
	}
	else if (true == GameEngineInput::IsDown("ArrowLeft"))
	{
		MoveWorldMap(int2::Left);
	}
	else if (true == GameEngineInput::IsDown("ArrowRight"))
	{
		MoveWorldMap(int2::Right);
	} 

	if (true == GameEngineInput::IsDown("LevelChangeSpace") || true == GameEngineInput::IsDown("LevelChangeEnter"))
	{
		if (nullptr == vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x])
		{
			MsgAssert("Nullptr 맵 데이터를 참조하려 했습니다");
			return;
		}

		if ("" == vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x]->GetPuzzleName())
		{
			return;
		}

		PuzzleLevel::SetPuzzleMapName(vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x]->GetPuzzleName());
		WorldMapFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangePuzzleLevel);
	}
}


void WorldMapLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	WorldMapFadeActor->Fade(FADE_STATE::FADEOUT);
}

void WorldMapLevel::MoveWorldMap(const int2& _Move)
{
	int2 NextPos = SelectPuzzlePos + _Move;

	if (0 > NextPos.x || MapSize.x <= NextPos.x)
	{
		return;
	}

	if (0 > NextPos.y || MapSize.y <= NextPos.y)
	{
		return;
	}

	SelectPuzzlePos = NextPos;

	if (nullptr == WorldMapSelectActor)
	{
		MsgAssert("WorldMapSelectActor를 생성하지 않았습니다");
		return;
	}

	WorldMapSelectActor->SetPos(vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x]->GetPos());
}
