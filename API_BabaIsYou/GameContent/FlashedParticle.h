#pragma once
#include "WiggleActor.h"

class GameEngineRender;
class WiggleRender;
class FlashedParticle : public WiggleActor
{
public:
	FlashedParticle();
	~FlashedParticle();

	FlashedParticle(const FlashedParticle& _Other) = delete;
	FlashedParticle(FlashedParticle&& _Other) noexcept = delete;
	FlashedParticle& operator=(const FlashedParticle& _Other) = delete;
	FlashedParticle& operator=(FlashedParticle&& _Other) noexcept = delete;

	void StartFlashed(float4 _MoveDir, float _MoveSpeed, float _WaitTime);

protected:
	void Start() override;
	void Update(float _DT) override;

private:
	WiggleRender* DotWiggle = nullptr;
	GameEngineRender* GlitteringRender = nullptr;

	float4 MoveDir = float4::Zero;
	float MoveSpeed = 0.0f;
	float CurWaitTime = 0.0f;
	float EndWaitTime = 0.0f;
};