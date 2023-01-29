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
#include "MapToolGridData.h"
#include "ContentDataLoader.h"

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

	if (false == GameEngineInput::IsKey("MapEscape"))
	{
		GameEngineInput::CreateKey("MapEscape", VK_ESCAPE);
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

	vecMapDatas.reserve(ContentConst::GRID_SIZE_Y);

	for (size_t y = 0; y < vecMapDatas.capacity(); y++)
	{
		vecMapDatas.push_back(std::vector<MapToolGridData*>());
		vecMapDatas[y].reserve(ContentConst::GRID_SIZE_X);

		for (size_t x = 0; x < vecMapDatas[y].capacity(); x++)
		{
			vecMapDatas[y].push_back(CreateActor<MapToolGridData>());
			vecMapDatas[y][x]->Off();
			vecMapDatas[y][x]->SetPos(
				{(ContentConst::ACTOR_SIZE.x * x) + ContentConst::ACTOR_SIZE.half().x,
				(ContentConst::ACTOR_SIZE.y * y) + ContentConst::ACTOR_SIZE.half().y});
		}
	}

	ResizeMap({10, 10});
	SelectBrush(MAPTOOL_BRUSH::PEN);
}

void MapToolLevel::Update(float _DT)
{
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
		ResizeMap(MapSize + int2{1, 0});
	}
	else if (true == MapSizeDownButtonX->IsUp())
	{
		ResizeMap(MapSize + int2{ -1, 0 });
	}
	else if (true == MapSizeUpButtonY->IsUp())
	{
		ResizeMap(MapSize + int2{ 0, 1 });
	}
	else if (true == MapSizeDownButtonY->IsUp())
	{
		ResizeMap(MapSize + int2{ 0, -1 });
	}
	else if (true == MapSaveButton->IsUp())
	{
		SaveMap();
	}
	else if (true == MapLoadButton->IsUp())
	{
		LoadMap();
	}
	else if (GameEngineInput::IsDown("MapEscape"))
	{
		MapToolFadeActor->Fade(FADE_STATE::FADEIN, ContentFunc::ChangeTitleLevel);
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
	MapToolFadeActor->Fade(FADE_STATE::FADEOUT);
}

void MapToolLevel::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	
}

MapToolGridData* MapToolLevel::SelectGrid()
{
	POINT MousePos = POINT{ 0, 0 };
	GetCursorPos(&MousePos);
	ScreenToClient(GameEngineWindow::GetHWnd(), &MousePos);

	MousePos.x += GetCameraPos().ix();
	MousePos.y += GetCameraPos().iy();

	int2 SelectIndex = int2::Zero;

	SelectIndex.x = static_cast<int>(MousePos.x / ContentConst::ACTOR_SIZE.x);
	SelectIndex.y = static_cast<int>(MousePos.y / ContentConst::ACTOR_SIZE.y);

	if (0 > SelectIndex.x || MapSize.x <= SelectIndex.x ||
		0 > SelectIndex.y || MapSize.y <= SelectIndex.y)
	{
		return nullptr;
	}
	else
	{
		return vecMapDatas[SelectIndex.y][SelectIndex.x];
	}
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
	MapToolGridData* SelectData =  SelectGrid();

	if (nullptr == SelectData)
	{
		return;
	}

	SelectData->SetIndex(Pallet->GetPenEnum());
}

void MapToolLevel::EraseMap()
{
	MapToolGridData* SelectData = SelectGrid();

	if (nullptr == SelectData)
	{
		return;
	}

	SelectData->SetIndex(-1);
}

void MapToolLevel::DisableMap()
{
	for (size_t y = 0; y < vecMapDatas.size(); y++)
	{
		for (size_t x = 0; x < vecMapDatas[y].size(); x++)
		{
			vecMapDatas[y][x]->Off();
		}
	}
}

void MapToolLevel::ActiveMap()
{
	for (size_t y = 0; y < MapSize.y; y++)
	{
		for (size_t x = 0; x < MapSize.x; x++)
		{
			vecMapDatas[y][x]->On();
		}
	}
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

	DisableMap();
	MapSize = _MapSize;

	float4 TotalGridSize = { ContentConst::ACTOR_SIZE.x * MapSize.x, ContentConst::ACTOR_SIZE.y * MapSize.y };	
	float4 WindowSize = GameEngineWindow::GetScreenSize();
	float4 DiffSize = WindowSize - TotalGridSize;

	GridBackActor->SetScale(TotalGridSize);
    SetCameraPos(-DiffSize.half());

	ActiveMap();
}


void MapToolLevel::SaveMap()
{
	std::vector<std::vector<int>> SaveData;
	SaveData.reserve(MapSize.y);

	for (size_t y = 0; y < MapSize.y; y++)
	{
		SaveData.push_back(std::vector<int>());
		SaveData[y].reserve(MapSize.x);

		for (size_t x = 0; x < MapSize.x; x++)
		{
			SaveData[y].push_back(-1);
			SaveData[y][x] = vecMapDatas[y][x]->GetIndex();
		}
	}

	ContentDataLoader::SaveMapData(ContentDataLoader::GetSaveFilePath(), SaveData);
}

void MapToolLevel::LoadMap()
{

	std::vector<std::vector<int>> LoadData;

	if (true == ContentDataLoader::LoadMapData(ContentDataLoader::GetOpenFilePath(), LoadData))
	{
		for (size_t y = 0; y < vecMapDatas.size(); y++)
		{
			for (size_t x = 0; x < vecMapDatas[y].size(); x++)
			{
				vecMapDatas[y][x]->SetIndex(-1);
				vecMapDatas[y][x]->Off();
			}
		}

		MapSize = { static_cast<int>(LoadData[0].size()), static_cast<int>(LoadData.size()) };
		ResizeMap(MapSize);

		for (size_t y = 0; y < MapSize.y; y++)
		{
			for (size_t x = 0; x < MapSize.x; x++)
			{
				vecMapDatas[y][x]->On();
				vecMapDatas[y][x]->SetIndex(LoadData[y][x]);
			}
		}
	}
}

void MapToolLevel::ActiveMainButton()
{
	PenBrushButton->On();
	EraseBrushButton->On();
	ObjectButton->On();
	MapSizeUpButtonY->On();
	MapSizeDownButtonY->On();	
	MapSizeUpButtonX->On();
	MapSizeDownButtonX->On();
	MapSaveButton->On();
	MapLoadButton->On();
}

void MapToolLevel::ActivePalletButton()
{
	PalletCloseButton->On();
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