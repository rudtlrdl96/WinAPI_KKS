#include "ParticleSystem.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ParticleActor.h"

ParticleSystem* ParticleSystem::CurLevelParticleSystem = nullptr;

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

ParticleActor* ParticleSystem::UseParticle(std::string _AnimName, PARTICLE_COLOR _Color, float4 _Size)
{
	if (0 == vecParticlePool.size())
	{
		CreateParticle();
	}

	ParticleActor* ReturnPtr = vecParticlePool.back();
	ReturnPtr->On();
	ReturnPtr->StartParticle(_AnimName, _Color, _Size);

	vecParticlePool.pop_back();
	return ReturnPtr;
}

void ParticleSystem::ReturnItem(ParticleActor* _Particle)
{
	_Particle->Off();
	vecParticlePool.push_back(_Particle);
}

ParticleSystem* ParticleSystem::GetLevelParticleSystem()
{
	if (nullptr == CurLevelParticleSystem)
	{
		MsgAssert("파티클 시스템을 생성하지 않고 사용하려 했습니다");
	}

	return CurLevelParticleSystem;
}

void ParticleSystem::Start()
{
	vecParticlePool.reserve(512);
	CreateParticle();
}

void ParticleSystem::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr != CurLevelParticleSystem)
	{
		MsgAssert("한개의 레벨에 파티클 시스템을 중복 생성하였습니다");
		return;
	}

	CurLevelParticleSystem = this;
}

void ParticleSystem::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	if (nullptr == CurLevelParticleSystem)
	{
		MsgAssert("한개의 레벨에 파티클 시스템을 중복 생성하였습니다");
		return;
	}

	CurLevelParticleSystem = nullptr;
}

void ParticleSystem::CreateParticle()
{
	for (size_t i = 0; i < 32; i++)
	{
		ParticleActor* CreateParticlePtr = GetLevel()->CreateActor<ParticleActor>();
		CreateParticlePtr->Off();
		vecParticlePool.push_back(CreateParticlePtr);
	}
}
