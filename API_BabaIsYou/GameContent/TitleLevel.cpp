#include "TitleLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEngineCore/GameEngineCore.h>

#include "FadeUI.h"
#include "TitleLogoUI.h"
#include "BlackBackUI.h"
#include "ContentFunc.h"
#include "ButtonUI.h"
#include "WiggleGridActor.h"
#include "WiggleMapToolActor.h"
#include "ContentDataLoader.h"
#include "ContentConst.h"
#include "WiggleRender.h"
#include "CameraSystem.h"
#include "SoundSystem.h"

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{
}

void ChangeLevel()
{

}

void TitleLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("Title");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("TitleLogo.BMP"))->Cut(1, 3);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("StartButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("MapToolButton.BMP"))->Cut(1, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("ExitButton.BMP"))->Cut(1, 2);

	GameEngineDirectory DataPath;

	DataPath.MoveParentToDirectory("Data");
	DataPath.Move("Data");
	DataPath.Move("Map");

	std::vector<std::vector<int>> LoadMapData;
	std::vector<std::vector<int>> LoadDirData;

	ContentDataLoader::LoadMapData(DataPath.GetPlusFileName("TitleBack").GetPathToString(), LoadMapData, LoadDirData);

	int2 LoadSize = { LoadMapData[0].size(), LoadMapData.size() };

	BackGroundActors = CreateActor<WiggleGridActor>();
	BackGroundActors->InitGrid(LoadSize);

	for (size_t y = 0; y < LoadMapData.size(); y++)
	{
		for (size_t x = 0; x < LoadMapData[y].size(); x++)
		{
			BackGroundActors->SetRender({x, y}, LoadMapData[y][x], static_cast<DIR_FLAG>(LoadDirData[y][x]));
		}
	}

	vecCameraMovePoint.resize(4);

	vecCameraMovePoint[0] = float4::Zero;

	float4 FirstPoint = float4::Zero;
	FirstPoint.x += LoadSize.x * ContentConst::ACTOR_SIZE.x - GameEngineWindow::GetScreenSize().x;
	FirstPoint.y += LoadSize.y * ContentConst::ACTOR_SIZE.y - GameEngineWindow::GetScreenSize().y;
	vecCameraMovePoint[1] = FirstPoint;

	float4 SecondPoint = float4::Zero;
	SecondPoint.x += LoadSize.x * ContentConst::ACTOR_SIZE.x - GameEngineWindow::GetScreenSize().x;
	vecCameraMovePoint[2] = SecondPoint;

	float4 ThirdPoint = float4::Zero;
	ThirdPoint.y += LoadSize.y * ContentConst::ACTOR_SIZE.y - GameEngineWindow::GetScreenSize().y;
	vecCameraMovePoint[3] = ThirdPoint;

	TitleFadeActor = CreateActor<FadeUI>();
	CreateActor<TitleLogoUI>();
	CreateActor<BlackBackUI>();
	CreateActor<CameraSystem>();
	SoundSystemPtr = CreateActor<SoundSystem>();

	vecTitleButtons.reserve(TB_COUNT);

	for (size_t i = 0; i < vecTitleButtons.capacity(); i++)
	{
		vecTitleButtons.push_back(CreateActor<ButtonUI>());
	}

	{
		vecTitleButtons[TB_GAME]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_GAME]->SetPos({640, 410});
		vecTitleButtons[TB_GAME]->SetImage("StartButton.BMP", { 400, 60 });
	}
	{
		vecTitleButtons[TB_MAPTOOL]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_MAPTOOL]->SetPos({ 640, 480 });
		vecTitleButtons[TB_MAPTOOL]->SetImage("MapToolButton.BMP", { 400, 60 });
	}
	{
		vecTitleButtons[TB_EXIT]->SetColSize({ 400, 60 });
		vecTitleButtons[TB_EXIT]->SetPos({ 640, 550 });
		vecTitleButtons[TB_EXIT]->SetImage("ExitButton.BMP", { 400, 60 });
	}

	ButtonBaba = CreateActor<WiggleMapToolActor>();
	ButtonBaba->SetRender("BABA", DIR_FLAG::RIGHT);
	ButtonBaba->GetRender()->CameraEffectOff();

	if (false == GameEngineInput::IsKey("MouseLeft"))
	{
		GameEngineInput::CreateKey("MouseLeft", VK_LBUTTON);
		GameEngineInput::CreateKey("ButtonUp", VK_UP);
		GameEngineInput::CreateKey("ButtonDown", VK_DOWN);
		GameEngineInput::CreateKey("ButtonUseSpace", VK_SPACE);
		GameEngineInput::CreateKey("ButtonUseEnter", VK_RETURN);
	}

	BaBaSetPos();
}

void TitleLevel::Update(float _DT)
{
	TitleCameraMove(_DT);

	if (nullptr == GetFocus())
	{
		return;
	}

	if (true == TitleFadeActor->IsProgress())
	{
		return;
	}

	if (true == GameEngineInput::IsDown("ButtonUp"))
	{
		ButtonUp();
	}
	else if (true == GameEngineInput::IsDown("ButtonDown"))
	{
		ButtonDown();
	}
	else if (true == GameEngineInput::IsDown("ButtonUseSpace") || true == GameEngineInput::IsDown("ButtonUseEnter"))
	{
		ButtonUse();
	}

	if (true == vecTitleButtons[TB_GAME]->IsUp())
	{
		TitleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeWorldmapLevel });
		SoundSystemPtr->BgmStop();
		SoundSystemPtr->Play("TitleOut.ogg");
	}
	else if (true == vecTitleButtons[TB_MAPTOOL]->IsUp())
	{
		TitleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeMapToolLevel });
		SoundSystemPtr->BgmStop();
	}
	else if (true == vecTitleButtons[TB_EXIT]->IsUp())
	{
		GameEngineWindow::AppOff();
	}
}

void TitleLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	TitleFadeActor->Fade({ .State = FADE_STATE::FADEOUT, .WaitTime = 0.5f });
	SoundSystemPtr->BgmPlay("Menu.ogg");
}

void TitleLevel::ButtonUp()
{
	if (0 >= SelectButton)
	{
		return;
	}

	--SelectButton;
	BaBaSetPos();
}

void TitleLevel::ButtonDown()
{
	if (vecTitleButtons.size() - 1 <= SelectButton)
	{
		return;
	}

	++SelectButton;
	BaBaSetPos();
}

void TitleLevel::ButtonUse()
{
	if (TB_GAME == SelectButton)
	{
		TitleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeWorldmapLevel });
		SoundSystemPtr->BgmStop();
		SoundSystemPtr->Play("TitleOut.ogg");
	}
	else if (TB_MAPTOOL == SelectButton)
	{
		TitleFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeMapToolLevel });
		SoundSystemPtr->BgmStop();
	}
	else if (TB_EXIT == SelectButton)
	{
		GameEngineWindow::AppOff();
	}
}

void TitleLevel::BaBaSetPos()
{
	ButtonBaba->SetPos(vecTitleButtons[SelectButton]->GetPos() - float4{230.0f, 0.0f});
}

void TitleLevel::TitleCameraMove(float _DT)
{
	CameraSystem* LevelCameraSystem = CameraSystem::GetLevelCameraSystem();

	if (false == LevelCameraSystem->IsCameraLerp())
	{
		size_t NextCameraIndex = CameraPointindex + 1;

		if (vecCameraMovePoint.size() <= NextCameraIndex)
		{
			NextCameraIndex = 0;
		}

		LevelCameraSystem->CameraLerp(vecCameraMovePoint[CameraPointindex], vecCameraMovePoint[NextCameraIndex],
			ContentConst::TITLE_CAMERA_SPEED / GetCameraPointDistance(CameraPointindex));

		CameraPointindex = NextCameraIndex;
	}
}

float TitleLevel::GetCameraPointDistance(size_t _Index)
{
	size_t NextCameraIndex = CameraPointindex + 1;

	if (vecCameraMovePoint.size() <= NextCameraIndex)
	{
		NextCameraIndex = 0;
	}

	return (vecCameraMovePoint[CameraPointindex] - vecCameraMovePoint[NextCameraIndex]).Size();
}
