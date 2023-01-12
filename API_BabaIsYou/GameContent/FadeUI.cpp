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

	PushFade(BoxImage, { -(ScreenCenter.x + 100.0f), ScreenCenter.y }, {-(ScreenCenter.x + 100.0f), 0.0f}, ScreenSize, ScreenSize);
	PushFade(BoxImage, { ScreenSize.x + ScreenCenter.x + 100.0f, ScreenCenter.y }, { ScreenCenter.x + 100.0f, 0.0f}, ScreenSize, ScreenSize);
	PushFade(BoxImage, { ScreenCenter.x, -(ScreenCenter.y + 100.0f) }, {0.0f, -(ScreenCenter.y + 100.0f)}, ScreenSize, ScreenSize);
	PushFade(BoxImage, { ScreenCenter.x, ScreenSize.y + ScreenCenter.y + 100.0f }, {0.0f, ScreenCenter.y + 100.0f}, ScreenSize, ScreenSize);

	float4 BitmapSize = { 256.0f, 256.0f };

	PushFade(CircleImage, { 670.0f, -250.0f }, float4::Zero, BitmapSize, { 300.0f, 300.0f });
	PushFade(CircleImage, { 900.0f, -250.0f }, float4::Zero, BitmapSize, { 300.0f, 300.0f });
	PushFade(CircleImage, { 470.0f, -250.0f }, float4::Zero, BitmapSize, { 300.0f, 300.0f });

	PushFade(CircleImage, { 610.0f, 970.0f }, float4::Zero, BitmapSize, { 300.0f, 300.0f });
	//vecFadePos.push_back(LerpFadeData(CircleImage, {BoxHalfSize.x, -128.0f}, ScreenCenter, { 200.0f, 200.0f }, {256.0f, 256.0f}));
	//vecFadePos.push_back(LerpFadeData(CircleImage, {BoxHalfSize.x, ScreenSize.y + 128.0f}, ScreenCenter, { 200.0f, 200.0f }, { 256.0f, 256.0f }));
}

void FadeUI::PushFade(GameEngineImage* _Image, const float4& _Start, const float4& _Interval, const float4& _BitmapSize, const float4& _Scale)
{
	if (nullptr == _Image)
	{
		MsgAssert("Null Image를 등록하려 했습니다");
		return;
	}

	float4 ScreenSize = GameEngineWindow::GetScreenSize();
	float4 ScreenCenter = GameEngineWindow::GetScreenSize().half();

	LerpFadeData LerpData;

	LerpData.Image = _Image;
	LerpData.Start = _Start;
	LerpData.BitSize = _BitmapSize;
	LerpData.Scale = _Scale;

	LerpData.Dest = ScreenCenter + _Interval;

	vecFadePos.push_back(LerpData);
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
		// Todo : DeltaTime&
		FadeScale += 0.0005f;

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
		for (const LerpFadeData& LerpData : vecFadePos)
		{
			if (nullptr == LerpData.Image)
			{
				MsgAssert("Null Image를 출력하려 했습니다");
				return;
			}

			GameEngineWindow::GetDoubleBufferImage()->TransCopy(
				LerpData.Image,
				Lerp(LerpData.Start, LerpData.Dest, FadeScale),
				LerpData.Scale,
				float4::Zero,
				LerpData.BitSize);
		}
		break;
	}
	case FadeUI::FADEOUT:
	{
		for (const LerpFadeData& LerpData : vecFadePos)
		{
			if (nullptr == LerpData.Image)
			{
				MsgAssert("Null Image를 출력하려 했습니다");
				return;
			}

			GameEngineWindow::GetDoubleBufferImage()->TransCopy(
				LerpData.Image,
				Lerp(LerpData.Dest, LerpData.Start, FadeScale),
				LerpData.Scale,
				float4::Zero,
				LerpData.BitSize);
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
