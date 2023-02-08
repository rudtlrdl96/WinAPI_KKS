#pragma once
#include <vector>
#include <GameEngineCore/GameEngineActor.h>

class FlashedParticle;
class AppearParticle : public GameEngineActor
{
public:
	AppearParticle();
	~AppearParticle();

	AppearParticle(const AppearParticle& _Other) = delete;
	AppearParticle(AppearParticle&& _Other) noexcept = delete;
	AppearParticle& operator=(const AppearParticle& _Other) = delete;
	AppearParticle& operator=(AppearParticle&& _Other) noexcept = delete;

	void StartParticle(int _Count, float _MinStart, float _MaxStart, float _MoveSpeed, float _MinWaitTime, float _MaxWaitTime);

protected:

private:
	std::vector<FlashedParticle*> vecParticles;
};