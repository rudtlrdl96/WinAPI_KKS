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
	TextRender = CreateRender("Text.BMP", _Order);
	TextRender->EffectCameraOff();
	TextRender->SetFrame(0);
}

void TextUI::SetText(char _Text)
{
	_Text = toupper(_Text);

	if ('0' <= _Text && '9' >= _Text)
	{
		TextRender->SetFrame(_Text - '0' + 1);
	}
	else if ('A' <= _Text && 'Z' >= _Text)
	{
		TextRender->SetFrame(11 + _Text - 'A');
	}
	else if ('?' == _Text)
	{
		TextRender->SetFrame(37);
	}
	else
	{
		TextRender->SetFrame(0);
	}
}

void TextUI::SetScale(const float4& _Scale)
{
	TextRender->SetScale(_Scale);
}
