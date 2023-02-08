#include "AppearParticle.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "FlashedParticle.h"
#include "ContentRand.h"

AppearParticle::AppearParticle()
{
}

AppearParticle::~AppearParticle()
{
}

void AppearParticle::StartParticle(int _Count, float _MinStart, float _MaxStart, float _MoveSpeed, float _MinWaitTime, float _MaxWaitTime)
{
	if (vecParticles.size() < _Count)
	{
		vecParticles.reserve(_Count);

		for (size_t i = vecParticles.size(); i < _Count; i++)
		{
			vecParticles.push_back(GetLevel()->CreateActor<FlashedParticle>());
		}
	}

	for (size_t i = 0; i < vecParticles.size(); i++)
	{
		vecParticles[i]->Off();
	}

	for (size_t i = 0; i < _Count; i++)
	{
		float4 RandDir = float4::Zero;
		float RandValue = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);

		RandDir.x = std::cos(RandValue);
		RandDir.y = std::sin(RandValue);

		float StartDis = ContentRand::RandFloat(_MinStart, _MaxStart);

		vecParticles[i]->SetPos(GetPos() + (RandDir * StartDis));
		vecParticles[i]->StartFlashed(RandDir, _MoveSpeed, ContentRand::RandFloat(_MinWaitTime, _MaxWaitTime));
		vecParticles[i]->On();
	}
}
