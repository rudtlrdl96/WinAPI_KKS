#include "FadeUI.h"
#include "ContentMath.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineResources.h>

FadeUI::FADE_STATE FadeUI::State = FADE_STATE::NONE;
float FadeUI::FadeScale = 0.0f;

FadeUI::FadeUI()
{
}

FadeUI::~FadeUI()
{
}

void FadeUI::Start()
{
	CircleImage = GameEngineResources::GetInst().ImageFind("FadeCircle.BMP");
	BoxImage = GameEngineResources::GetInst().ImageFind("Fade.BMP");

	float4 ScreenSize = GameEngineWindow::GetScreenSize();
	float4 ScreenCenter = GameEngineWindow::GetScreenSize().half();
	float4 BoxHalfSize = { 640.0f, 360.0f };

	vecBoxPos.push_back(std::make_pair(float4{ -BoxHalfSize.x, ScreenCenter.y}, ScreenCenter));
	vecBoxPos.push_back(std::make_pair(float4{ ScreenSize.x + BoxHalfSize.x, ScreenCenter.y }, ScreenCenter));
	vecBoxPos.push_back(std::make_pair(float4{ ScreenCenter.x, -BoxHalfSize.y}, ScreenCenter));
	vecBoxPos.push_back(std::make_pair(float4{ ScreenCenter.x, ScreenSize.y + BoxHalfSize.y}, ScreenCenter));
}

void FadeUI::Update()
{
	switch (State)
	{
	case FadeUI::NONE:
	{
		return;
	}
	case FadeUI::FADEIN:
	case FadeUI::FADEOUT:
	{
		// Todo : DeltaTime
		FadeScale += 0.005f;

		if (FadeScale >= 1.0f)
		{
			FadeScale = 1.0f;
		}
		break;
	}
	default:
		MsgAssert("잘못된 FadeUI State 입니다");
		break;
	}
}

void FadeUI::Render()
{
	switch (State)
	{
	case FadeUI::NONE:
	{	
		return;
	}
	case FadeUI::FADEIN:
	{
		for (const std::pair<float4, float4>& LerpData : vecBoxPos)
		{
			GameEngineWindow::GetDoubleBufferImage()->TransCopy(
				BoxImage,
				Lerp(LerpData.first, LerpData.second, FadeScale),
				{1280.0f, 720.0f},
				float4::Zero,
				{1280.0f, 720.0f});
		}

		//for (size_t i = 0; i < vecCirclePos.size(); i++)
		//{
		//
		//}

		break;
	}
	case FadeUI::FADEOUT:
	{
		for (const std::pair<float4, float4>& LerpData : vecBoxPos)
		{
			GameEngineWindow::GetDoubleBufferImage()->TransCopy(
				BoxImage,
				Lerp(LerpData.second, LerpData.first, FadeScale),
				{ 1280.0f, 720.0f },
				float4::Zero,
				{ 1280.0f, 720.0f });
		}
		break;
	}
	default:
		MsgAssert("잘못된 FadeUI State 입니다");
		break;
	}
}

void FadeUI::FadeIn()
{
	State = FADE_STATE::FADEIN;
	FadeScale = 0.0f;
}

void FadeUI::FadeOut()
{
	State = FADE_STATE::FADEOUT;
	FadeScale = 0.0f;
}

void FadeUI::Reset()
{
	State = FADE_STATE::NONE;
	FadeScale = 0.0f;
}
