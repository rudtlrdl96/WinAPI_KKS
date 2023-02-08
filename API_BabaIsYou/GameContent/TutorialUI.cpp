#include "TutorialUI.h"
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

TutorialUI::TutorialUI()
{
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::SetTutorialImage(const std::string_view& _ImageName, float4 _Scale)
{
	TutorialRender->SetImage(_ImageName);
	TutorialRender->SetScale(_Scale);
	TutorialRender->On();
}

void TutorialUI::Start()
{
	TutorialRender = CreateRender(RENDER_ORDER::UIIMAGE);
	TutorialRender->Off();
}
