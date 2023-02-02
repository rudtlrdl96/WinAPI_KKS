#include "WiggleStringUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include "TextUI.h"
#include "WiggleTextUI.h"

WiggleStringUI::WiggleStringUI()
{

}

WiggleStringUI::~WiggleStringUI()
{

}

void WiggleStringUI::CreateTextActor(char _Text)
{
	WiggleTextUI* ReturnText = GetLevel()->CreateActor<WiggleTextUI>();
	ReturnText->Init(TextRenderOrder);
	ReturnText->SetText(_Text);
	vecTextActors.push_back(ReturnText);
}