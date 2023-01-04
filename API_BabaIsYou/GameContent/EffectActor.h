#pragma once
#include <GameEngineCore/GameEngineActor.h>

class EffectActor : public GameEngineActor
{
public:
	EffectActor();
	~EffectActor();

	EffectActor(const EffectActor& _Other) = delete;
	EffectActor(EffectActor&& _Other) noexcept = delete;
	EffectActor& operator=(const EffectActor& _Other) = delete;
	EffectActor& operator=(EffectActor&& _Other) noexcept = delete;

protected:

private:

};