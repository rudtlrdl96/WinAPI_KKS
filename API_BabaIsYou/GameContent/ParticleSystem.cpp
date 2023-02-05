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
		MsgAssert("��ƼŬ �ý����� �������� �ʰ� ����Ϸ� �߽��ϴ�");
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
		MsgAssert("�Ѱ��� ������ ��ƼŬ �ý����� �ߺ� �����Ͽ����ϴ�");
		return;
	}

	CurLevelParticleSystem = this;
}

void ParticleSystem::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	if (nullptr == CurLevelParticleSystem)
	{
		MsgAssert("�Ѱ��� ������ ��ƼŬ �ý����� �ߺ� �����Ͽ����ϴ�");
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
