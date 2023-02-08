#include "DeathUndoUI.h"
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

DeathUndoUI::DeathUndoUI()
{
}

DeathUndoUI::~DeathUndoUI()
{
}

void DeathUndoUI::Start()
{
	GameEngineRender* Renderer = CreateRender("DeathUndoUI.BMP", RENDER_ORDER::UIIMAGE);
	Renderer->SetScale({ 120, 39 });
	Renderer->EffectCameraOff();
}
