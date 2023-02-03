#include "TextUI.h"
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

TextUI::TextUI()
{
}

TextUI::~TextUI()
{
}

void TextUI::Init(int _Order)
{
	if (nullptr != TextRender)
	{
		MsgAssert("TextUI를 중복 초기화 하였습니다");
		return;
	}

	TextRender = CreateRender("Text.BMP", _Order);
	TextRender->EffectCameraOff();
	TextRender->SetFrame(0);
}

void TextUI::Update(float _DT)
{
	if (true == IsSizeLerp)
	{
		LerpSizeRatio += _DT / SizeTime;
		float4 LerpSize = float4::LerpClamp(LerpStartSize, LerpEndSize, LerpSizeRatio);

		SetScale(LerpSize);

		if (1.0f <= LerpSizeRatio)
		{
			IsSizeLerp = false;
			LerpSizeRatio = 0.0f;

			if (true == IsSizeLerpEndOff)
			{
				Off(); 
			}
		}
		return;
	}
	else if (true == IsShakeValue)
	{
		LerpShakeRatio += _DT / ShakeTime;
		SetPos(float4::LerpClamp(ShakeStartPos, ShakeEndPos, LerpShakeRatio));

		if (1.0f <= LerpShakeRatio)
		{
			LerpShakeRatio = 0.0f;

			float4 SwapPos = ShakeStartPos;
			ShakeStartPos = ShakeEndPos;
			ShakeEndPos = SwapPos;
		}
	}
}

void TextUI::SetText(char _Text)
{
	CurText = toupper(_Text);

	int ColorIndex = static_cast<int>(TextColor) * 38;

	if ('0' <= CurText && '9' >= CurText)
	{
		TextRender->SetFrame(ColorIndex + CurText - '0' + 1);
	}
	else if ('A' <= CurText && 'Z' >= CurText)
	{
		TextRender->SetFrame(ColorIndex + 11 + CurText - 'A');
	}
	else if ('?' == CurText)
	{
		TextRender->SetFrame(ColorIndex + 37);
	}
	else
	{
		TextRender->SetFrame(ColorIndex + 0);
	}
}

void TextUI::SetScale(const float4& _Scale)
{
	TextRender->SetScale(_Scale);
	SetText(CurText);
}


void TextUI::LerpSize(float4 _Start, float4 _End, float _Time, bool _IsEndOff)
{
	IsSizeLerp = true;
	IsSizeLerpEndOff = _IsEndOff;

	LerpStartSize = _Start;
	LerpEndSize = _End;

	LerpSizeRatio = 0.0f;
	SizeTime = _Time;
}

void TextUI::ShakeText(float4 _Dir, float _Time)
{
	IsShakeValue = true;
	CurPos = GetPos();

	ShakeStartPos = CurPos;
	ShakeEndPos = ShakeStartPos + _Dir;
	ShakeTime = _Time;
	LerpShakeRatio = 0.0f;
}

void TextUI::ResetShake()
{
	if (true == IsShakeValue)
	{
		SetPos(CurPos);
	}

	IsShakeValue = false;
}