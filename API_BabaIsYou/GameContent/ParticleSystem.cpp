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

ParticleActor* ParticleSystem::UseParticle(std::string _AnimName, PARTICLE_COLOR _Color, float4 _Pos, float4 _Size)
{
	if (0 == vecWaitParticles.size())
	{
		CreateParticle();
	}

	ParticleActor* ReturnPtr = vecWaitParticles.back();
	ReturnPtr->StartParticle(_AnimName, _Color, _Size);
	ReturnPtr->SetPos(_Pos);
	ReturnPtr->On();

	vecWaitParticles.pop_back();
	return ReturnPtr;
}

void ParticleSystem::ReturnItem(ParticleActor* _Particle)
{
	_Particle->Off();
	vecWaitParticles.push_back(_Particle);
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
	vecWaitParticles.reserve(512);
	vecAllParticles.reserve(512);
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

	for (size_t i = 0; i < vecAllParticles.size(); i++)
	{
		vecAllParticles[i]->ParticleReturn();
	}

	CurLevelParticleSystem = nullptr;
}

void ParticleSystem::CreateParticle()
{
	for (size_t i = 0; i < 32; i++)
	{
		ParticleActor* CreateParticlePtr = GetLevel()->CreateActor<ParticleActor>();
		CreateParticlePtr->Off();
		vecWaitParticles.push_back(CreateParticlePtr);
		vecAllParticles.push_back(CreateParticlePtr);
	}
}
