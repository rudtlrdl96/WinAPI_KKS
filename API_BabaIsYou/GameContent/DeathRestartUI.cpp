#include "DeathRestartUI.h"
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

DeathRestartUI::DeathRestartUI()
{
}

DeathRestartUI::~DeathRestartUI()
{
}

void DeathRestartUI::Start()
{
	GameEngineRender* Renderer = CreateRender("DeathRestartUI.BMP", RENDER_ORDER::UIIMAGE);
	Renderer->SetScale({ 130, 39 });
	Renderer->EffectCameraOff();
}
