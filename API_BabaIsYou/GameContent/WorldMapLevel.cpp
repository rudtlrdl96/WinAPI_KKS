#include "WorldMapLevel.h"

#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>
#include "MapBackgroundUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"
#include "ContentEnum.h"
#include "GrayBackUI.h"
#include "WorldMapActor.h"
#include "PuzzleLevel.h"
#include "WorldMapSelect.h"
#include "StringUI.h"
#include "WiggleMapToolActor.h"

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
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMapActorBack.BMP"))->Cut(2, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMapSelect.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WorldMapLine.BMP"))->Cut(16, 3);

	WorldMapFadeActor = CreateActor<FadeUI>();
	CreateActor<MapBackgroundUI>();
	CreateActor<GrayBackUI>();
	InfoStringUI = CreateActor<StringUI>();
	InfoStringUI->SetPos({ 10, 16 });
	InfoStringUI->SetFontSize({ 28, 28 });
	InfoStringUI->SetFontInterval({13, 17});

	{
		WiggleMapToolActor* BaBaText = CreateActor<WiggleMapToolActor>();
		BaBaText->SetRender("BABA_TEXT", DIR_FLAG::RIGHT);
		BaBaText->SetPos({ 1149, 605 });
		WiggleMapToolActor* IsText1  = CreateActor<WiggleMapToolActor>();
		IsText1->SetRender("IS", DIR_FLAG::RIGHT);
		IsText1->SetPos({ 1182, 605 });
		WiggleMapToolActor* YouText  = CreateActor<WiggleMapToolActor>();
		YouText->SetRender("YOU", DIR_FLAG::RIGHT);
		YouText->SetPos({ 1215, 605 });
		WiggleMapToolActor* FlagText = CreateActor<WiggleMapToolActor>();
		FlagText->SetRender("FLAG_TEXT", DIR_FLAG::RIGHT);
		FlagText->SetPos({ 1149, 665 });
		WiggleMapToolActor* IsText2  = CreateActor<WiggleMapToolActor>();
		IsText2->SetRender("IS", DIR_FLAG::RIGHT);
		IsText2->SetPos({ 1182, 665 });
		WiggleMapToolActor* WinText  = CreateActor<WiggleMapToolActor>();
		WinText->SetRender("WIN", DIR_FLAG::RIGHT);
		WinText->SetPos({ 1215, 665 });
	}

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

	vecWorldMapDatas[0][0]->InitWorldMapActor("Stage_0_0", "BABA IS YOU", 0);
	vecWorldMapDatas[2][1]->InitWorldMapActor("Stage_0_1", "WHERE DO I GO?", 1);
	vecWorldMapDatas[3][1]->InitWorldMapActor("Stage_0_2", "NOW WHAT IS THIS?", 2);
	vecWorldMapDatas[2][2]->InitWorldMapActor("Stage_0_3", "OUT OF REACH", 3);
	vecWorldMapDatas[3][2]->InitWorldMapActor("Stage_0_4", "STILL OUT OF REACH", 4);
	vecWorldMapDatas[4][1]->InitWorldMapActor("Stage_0_5", "VOLCALO", 5);
	vecWorldMapDatas[3][3]->InitWorldMapActor("Stage_0_6", "OFF LIMITS", 6);
	vecWorldMapDatas[4][2]->InitWorldMapActor("Stage_0_7", "GRASS YARD", 7);

	vecWorldMapDatas[0][1]->InitWorldMapActorLine();
	vecWorldMapDatas[1][1]->InitWorldMapActorLine();
	vecWorldMapDatas[4][3]->InitWorldMapActorLine();
	vecWorldMapDatas[2][3]->InitWorldMapActorLine();

	for (int y = 0; y < vecWorldMapDatas.size(); y++)
	{
		for (int x = 0; x < vecWorldMapDatas[y].size(); x++)
		{
			LineActorInit({x, y});
		}
	}

	MoveWorldMap(int2::Zero);
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

	if (false == vecWorldMapDatas[NextPos.y][NextPos.x]->IsWalkable())
	{
		return;
	}

	SelectPuzzlePos = NextPos;

	if (nullptr == WorldMapSelectActor)
	{
		MsgAssert("WorldMapSelectActor를 생성하지 않았습니다");
		return;
	}

	InfoStringUI->WriteText(vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x]->GetPuzzleInfo());
	WorldMapSelectActor->SetPos(vecWorldMapDatas[SelectPuzzlePos.y][SelectPuzzlePos.x]->GetPos());
}


void WorldMapLevel::LineActorInit(const int2& _Pos)
{
	if (true == MapOverCheck(_Pos))
	{
		MsgAssert("월드맵을 벗어나 접근하려 했습니다.");
		return;
	}

	if (false == vecWorldMapDatas[_Pos.y][_Pos.x]->IsLine())
	{
		return;
	}

	int DirKey = 0;

	int2 CheckPos = _Pos + int2::Left;
	if (false == MapOverCheck(CheckPos) && true == vecWorldMapDatas[CheckPos.y][CheckPos.x]->IsWalkable())
	{
		DirKey |= DIR_FLAG::LEFT;
	}

	CheckPos = _Pos + int2::Right;
	if (false == MapOverCheck(CheckPos) && true == vecWorldMapDatas[CheckPos.y][CheckPos.x]->IsWalkable())
	{
		DirKey |= DIR_FLAG::RIGHT;
	}	

	CheckPos = _Pos + int2::Up;
	if (false == MapOverCheck(CheckPos) && true == vecWorldMapDatas[CheckPos.y][CheckPos.x]->IsWalkable())
	{
		DirKey |= DIR_FLAG::DOWN;
	}	

	CheckPos = _Pos + int2::Down;
	if (false == MapOverCheck(CheckPos) && true == vecWorldMapDatas[CheckPos.y][CheckPos.x]->IsWalkable())
	{
		DirKey |= DIR_FLAG::UP;
	}

	vecWorldMapDatas[_Pos.y][_Pos.x]->SetTileIndex(DirKey);
}

bool WorldMapLevel::MapOverCheck(const int2& _Pos) const
{
	if (0 > _Pos.x || MapSize.x <= _Pos.x)
	{
		return true;
	}

	if (0 > _Pos.y || MapSize.y <= _Pos.y)
	{
		return true;
	}

	return false;
}