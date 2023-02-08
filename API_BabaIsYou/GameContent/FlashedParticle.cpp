#include "FlashedParticle.h"
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"
#include "WiggleRender.h"

FlashedParticle::FlashedParticle()
{
}

FlashedParticle::~FlashedParticle()
{
}

void FlashedParticle::Start()
{

	DotWiggle = InitWiggleRender({
	.FileName = "WiggleDot.BMP",
	.Pos = float4::Zero,
	.Scale = {15, 15},
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::UIIMAGE),
	.BitmapInterval = 1, 
	.IsUI = true });

	DotWiggle->ActiveWiggle();
	DotWiggle->RenderOff();

	GlitteringRender = CreateRender("FlashedParticle.BMP", RENDER_ORDER::UIIMAGE);
	GlitteringRender->SetScale({ 15, 15 });
	GlitteringRender->CreateAnimation({
	.AnimationName = "Flashed",
	.ImageName = "FlashedParticle.BMP",
	.Start = 0,
	.End = 1,
	.InterTime = 0.05f,
	.Loop = false });
	GlitteringRender->EffectCameraOff();
	GlitteringRender->Off();

	Off();
}

void FlashedParticle::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (CurWaitTime >= EndWaitTime)
	{
		if (true == GlitteringRender->IsAnimationEnd())
		{
			GlitteringRender->Off();
			DotWiggle->RenderOff();
			Off();
		}
	}
	else
	{
		CurWaitTime += _DT;

		if (CurWaitTime >= EndWaitTime)
		{
			DotWiggle->RenderOff();
			GlitteringRender->On();
			GlitteringRender->ChangeAnimation("Flashed", true);
		}
	}

	SetMove(MoveDir * MoveSpeed * _DT);
}

void FlashedParticle::StartFlashed(float4 _MoveDir, float _MoveSpeed, float _WaitTime)
{
	MoveDir = _MoveDir;
	MoveSpeed = _MoveSpeed;
	CurWaitTime = 0.0f;
	EndWaitTime = _WaitTime;

	DotWiggle->SetStartIndex(0);
	DotWiggle->RenderOn();

	GlitteringRender->Off();
	On();
}