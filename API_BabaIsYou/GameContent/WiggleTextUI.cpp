#include "WiggleTextUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentEnum.h"
#include "WiggleRender.h"

WiggleTextUI::WiggleTextUI()
{
}

WiggleTextUI::~WiggleTextUI()
{
}

void WiggleTextUI::Start()
{
	InitWiggleRender({
	.FileName = "WiggleText.BMP",
	.Pos = float4::Zero,
	.Scale = {10, 10},
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::TEXTUI),
	.BitmapInterval = 37,
	.IsUI = true });
}

void WiggleTextUI::SetText(char _Text)
{
	if (nullptr == GetWiggleRender())
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 참조하려 했습니다");
		return;
	}

	_Text = toupper(_Text);

	if ('0' <= _Text && '9' >= _Text)
	{
		GetWiggleRender()->SetStartIndex(_Text - '0' + 1);
	}
	else if ('A' <= _Text && 'Z' >= _Text)
	{
		GetWiggleRender()->SetStartIndex(11 + _Text - 'A');
	}
	else
	{
		GetWiggleRender()->SetStartIndex(0);
	}
}

void WiggleTextUI::SetScale(const float4& _Scale)
{
	GetWiggleRender()->SetScale(_Scale);
}
