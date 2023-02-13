#include "MapToolLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineResources.h>
#include "GrayBackUI.h"
#include "FadeUI.h"
#include "ContentFunc.h"
#include "ContentConst.h"
#include "BlackBackUI.h"
#include "PalletActor.h"
#include "ButtonUI.h"
#include "WiggleGridActor.h"
#include "ContentDataLoader.h"
#include "CameraSystem.h"
#include "SoundSystem.h"

MapToolLevel::MapToolLevel() :
	MapToolFadeActor(nullptr)
{
}

MapToolLevel::~MapToolLevel()
{
}

void MapToolLevel::Loading()
{
	CreateActor<GrayBackUI>();
	MapToolFadeActor = CreateActor<FadeUI>();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("MapTool");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Pallet.BMP"))->Cut(3, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("SelectPallet.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("CloseButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("ObjectsButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("PenBrushButton.BMP"))->Cut(2, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("EraseBrushButton.BMP"))->Cut(2, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapSizePlusButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapSizeMinusButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapToolSave.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapToolLoad.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DirUpButton.BMP"))->Cut(2, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DirDownButton.BMP"))->Cut(2, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DirLeftButton.BMP"))->Cut(2, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("DirRightButton.BMP"))->Cut(2, 2);

	GameEngineDirectory SoundDir;

	SoundDir.MoveParentToDirectory("ContentsResources");
	SoundDir.Move("ContentsResources");
	SoundDir.Move("Sound");
	SoundDir.Move("BGM");

	SoundSystemPtr = CreateActor<SoundSystem>();
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("editorsong.ogg"), SOUND_GROUP::BGM);

	if (false == GameEngineInput::IsKey("MapEscape"))
	{
		GameEngineInput::CreateKey("MapEscape", VK_ESCAPE);
		GameEngineInput::CreateKey("TempLeft", VK_LEFT);
		GameEngineInput::CreateKey("TempRight", VK_RIGHT);
		GameEngineInput::CreateKey("TempUp", VK_UP);
		GameEngineInput::CreateKey("TempDown", VK_DOWN);
	}

	GridBackActor = CreateActor<BlackBackUI>();
	Pallet = CreateActor<PalletActor>();
	Pallet->DisablePallet();

	PalletCloseButton = CreateActor<ButtonUI>();
	PalletCloseButton->SetImage("CloseButton.BMP", { 80, 80 });
	PalletCloseButton->SetColSize({ 80, 80 });
	PalletCloseButton->SetPos({ 1200, 60 });
	PalletCloseButton->Off();

	ObjectButton = CreateActor<ButtonUI>();
	ObjectButton->SetImage("ObjectsButton.BMP", {160, 40});
	ObjectButton->SetPos({ 1180, 690});

	PenBrushButton = CreateActor<ButtonUI>();
	PenBrushButton->SetImage("PenBrushButton.BMP", {60, 60});
	PenBrushButton->SetPos({ 1250, 50 });

	EraseBrushButton = CreateActor<ButtonUI>();
	EraseBrushButton->SetImage("EraseBrushButton.BMP", {60, 60});
	EraseBrushButton->SetPos({ 1250, 120 });

	MapSizeUpButtonY = CreateActor<ButtonUI>();
	MapSizeUpButtonY->SetImage("MapSizePlusButton.BMP", {80, 40});
	MapSizeUpButtonY->SetPos({ 1050, 690 });

	MapSizeDownButtonY = CreateActor<ButtonUI>();
	MapSizeDownButtonY->SetImage("MapSizeMinusButton.BMP", {80, 40});
	MapSizeDownButtonY->SetPos({ 960, 690 });

	MapSizeUpButtonX = CreateActor<ButtonUI>();
	MapSizeUpButtonX->SetImage("MapSizePlusButton.BMP", { 80, 40 });
	MapSizeUpButtonX->SetPos({ 870, 690 });

	MapSizeDownButtonX = CreateActor<ButtonUI>();
	MapSizeDownButtonX->SetImage("MapSizeMinusButton.BMP", { 80, 40 });
	MapSizeDownButtonX->SetPos({ 780, 690 });

	MapSaveButton = CreateActor<ButtonUI>();
	MapSaveButton->SetImage("MapToolSave.BMP", { 80, 40 });
	MapSaveButton->SetPos({ 50, 690 });

	MapLoadButton = CreateActor<ButtonUI>();
	MapLoadButton->SetImage("MapToolLoad.BMP", { 80, 40 });
	MapLoadButton->SetPos({ 140, 690 });

	WiggleGridActors = CreateActor<WiggleGridActor>();
	WiggleGridActors->InitGrid(ContentConst::GRID_SIZE);

	CameraSystemPtr = CreateActor<CameraSystem>();

	ResizeMap({10, 10});
	SelectBrush(MAPTOOL_BRUSH::PEN);
}

void MapToolLevel::Update(float _DT)
{
	if (1.0f > _DT)
	{
		SaveLoadWaitTime -= _DT;
	}

	if (0.0f < SaveLoadWaitTime)
	{
		return;
	}

	if (nullptr == GetFocus())
	{
		return;
	}

	if (true == PalletCloseButton->IsUp())
	{
		Pallet->DisablePallet();
		ActiveMainButton();
		DisablePalletButton();
	}

	if (true == Pallet->IsUpdate())
	{
		return;
	}
	
	if (true == MapToolFadeActor->IsProgress())
	{
		return;
	}

	if (true == ObjectButton->IsUp())
	{
		Pallet->ActivePallet();
		DisableMainButton();
		ActivePalletButton();
	}
	else if (true == PenBrushButton->IsUp())
	{
		SelectBrush(MAPTOOL_BRUSH::PEN);
	}
	else if (true == EraseBrushButton->IsUp())
	{
		SelectBrush(MAPTOOL_BRUSH::ERASE);
	}
	else if (true == MapSizeUpButtonX->IsUp())
	{
		ResizeMap(WiggleGridActors->GetGridSize() + int2{1, 0});
	}
	else if (true == MapSizeDownButtonX->IsUp())
	{
		ResizeMap(WiggleGridActors->GetGridSize() + int2{ -1, 0 });
	}
	else if (true == MapSizeUpButtonY->IsUp())
	{
		ResizeMap(WiggleGridActors->GetGridSize() + int2{ 0, 1 });
	}
	else if (true == MapSizeDownButtonY->IsUp())
	{
		ResizeMap(WiggleGridActors->GetGridSize() + int2{ 0, -1 });
	}
	else if (true == MapSaveButton->IsUp())
	{
		SaveMap();
		return;
	}
	else if (true == MapLoadButton->IsUp())
	{
		LoadMap();
		return;
	}
	else if (GameEngineInput::IsDown("MapEscape"))
	{
		MapToolFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeTitleLevel });
	}
	else if (GameEngineInput::IsPress("MouseLeft"))
	{
		if (MAPTOOL_BRUSH::PEN == BrushType)
		{
			DrawMap();
		}
		else if (MAPTOOL_BRUSH::ERASE == BrushType)
		{
			EraseMap();
		}
	}
}

void MapToolLevel::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	MapToolFadeActor->Fade({ .State = FADE_STATE::FADEOUT, .WaitTime = 0.5f});
	SoundSystemPtr->BgmPlay("editorsong.ogg");
}

void MapToolLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	
}

int2 MapToolLevel::SelectGrid()
{
	POINT MousePos = POINT{ 0, 0 };
	GetCursorPos(&MousePos);
	ScreenToClient(GameEngineWindow::GetHWnd(), &MousePos);

	MousePos.x += GetCameraPos().ix();
	MousePos.y += GetCameraPos().iy();

	int2 SelectIndex = int2::Zero;

	SelectIndex.x = static_cast<int>(MousePos.x / ContentConst::ACTOR_SIZE.x);
	SelectIndex.y = static_cast<int>(MousePos.y / ContentConst::ACTOR_SIZE.y);

	return SelectIndex;
}

void MapToolLevel::SelectBrush(MAPTOOL_BRUSH _Brush)
{
	BrushType = _Brush;

	PenBrushButton->SetSelect(false);
	EraseBrushButton->SetSelect(false);

	switch (BrushType)
	{
	case MapToolLevel::MAPTOOL_BRUSH::PEN:
		PenBrushButton->SetSelect(true);
		break;
	case MapToolLevel::MAPTOOL_BRUSH::ERASE:
		EraseBrushButton->SetSelect(true);
		break;
	}
}

void MapToolLevel::DrawMap()
{
	int2 SelectIndex = SelectGrid();

	if (true == WiggleGridActors->IsOver(SelectIndex))
	{
		return;
	}

	if (Pallet->GetPenEnum() != WiggleGridActors->GetActorEnum(SelectIndex))
	{
		SoundSystemPtr->Play("MapToolPen.ogg");
	}

	WiggleGridActors->SetRender(SelectIndex, Pallet->GetPenEnum(), Pallet->GetPalletDir());
}

void MapToolLevel::EraseMap()
{
	int2 SelectIndex = SelectGrid();

	if (true == WiggleGridActors->IsOver(SelectIndex))
	{
		return;
	}

	if (-1 != WiggleGridActors->GetActorEnum(SelectIndex))
	{
		SoundSystemPtr->Play("MapToolPen.ogg");
	}

	WiggleGridActors->SetRender(SelectIndex, -1, Pallet->GetPalletDir());
}

void MapToolLevel::ResizeMap(const int2& _MapSize)
{
	if (1 > _MapSize.x || ContentConst::GRID_SIZE_X < _MapSize.x)
	{
		return;
	}

	if (1 > _MapSize.y || ContentConst::GRID_SIZE_Y < _MapSize.y)
	{
		return;
	}

	WiggleGridActors->DisableGridActors();
	WiggleGridActors->SetGridSize(_MapSize);

	float4 TotalGridSize = { ContentConst::ACTOR_SIZE.x * _MapSize.x, ContentConst::ACTOR_SIZE.y * _MapSize.y };
	float4 WindowSize = GameEngineWindow::GetScreenSize();
	float4 DiffSize = WindowSize - TotalGridSize;

	GridBackActor->SetScale(TotalGridSize);
	CameraSystemPtr->SetCameraPos(-DiffSize.half());

	WiggleGridActors->ActiveGridActors();
}


void MapToolLevel::SaveMap()
{
	int2 MapSize = WiggleGridActors->GetGridSize();

	std::vector<std::vector<int>> SaveData;
	std::vector<std::vector<int>> SaveDir;
	SaveData.reserve(MapSize.y);
	SaveDir.reserve(MapSize.y);

	for (size_t y = 0; y < MapSize.y; y++)
	{
		SaveData.push_back(std::vector<int>());
		SaveDir.push_back(std::vector<int>());
		SaveData[y].reserve(MapSize.x);
		SaveDir[y].reserve(MapSize.x);

		for (size_t x = 0; x < MapSize.x; x++)
		{
			SaveData[y].push_back(WiggleGridActors->GetActorEnum({x, y}));
			SaveDir[y].push_back(WiggleGridActors->GetDir({x, y}));
		}
	}

	ContentDataLoader::SaveMapData(ContentDataLoader::GetSaveFilePath(), SaveData, SaveDir);
	SaveLoadWaitTime = 0.5f;
}

void MapToolLevel::LoadMap()
{
	std::vector<std::vector<int>> LoadData;
	std::vector<std::vector<int>> LoadDir;
	
	if (true == ContentDataLoader::LoadMapData(ContentDataLoader::GetOpenFilePath(), LoadData, LoadDir))
	{
		WiggleGridActors->ResetGridActors();
		ResizeMap({ static_cast<int>(LoadData[0].size()), static_cast<int>(LoadData.size()) });

		int2 MapSize = WiggleGridActors->GetGridSize();

		for (size_t y = 0; y < MapSize.y; y++)
		{
			for (size_t x = 0; x < MapSize.x; x++)
			{
				WiggleGridActors->SetRender({x, y}, LoadData[y][x], static_cast<DIR_FLAG>(LoadDir[y][x]));
			}
		}

		for (size_t y = 0; y < MapSize.y; y++)
		{
			for (size_t x = 0; x < MapSize.x; x++)
			{
				WiggleGridActors->TileCheck({ x, y });
			}
		}
	}

	SaveLoadWaitTime = 1.0f;
}

void MapToolLevel::ActiveMainButton()
{
	PenBrushButton->ButtonOn();
	EraseBrushButton->ButtonOn();
	ObjectButton->ButtonOn();
	MapSizeUpButtonY->ButtonOn();
	MapSizeDownButtonY->ButtonOn();
	MapSizeUpButtonX->ButtonOn();
	MapSizeDownButtonX->ButtonOn();
	MapSaveButton->ButtonOn();
	MapLoadButton->ButtonOn();
}

void MapToolLevel::ActivePalletButton()
{
	PalletCloseButton->ButtonOn();
}

void MapToolLevel::DisableMainButton()
{
	PenBrushButton->ButtonOff();
	EraseBrushButton->ButtonOff();
	ObjectButton->ButtonOff();
	MapSizeUpButtonY->ButtonOff();
	MapSizeDownButtonY->ButtonOff();
	MapSizeUpButtonX->ButtonOff();
	MapSizeDownButtonX->ButtonOff();
	MapSaveButton->ButtonOff();
	MapLoadButton->ButtonOff();
}

void MapToolLevel::DisablePalletButton()
{
	PalletCloseButton->ButtonOff();
}
