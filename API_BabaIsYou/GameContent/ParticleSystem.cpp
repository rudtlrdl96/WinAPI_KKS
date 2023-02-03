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
		MsgAssert("��ƼŬ �ý����� �������� �ʰ� ����Ϸ� �߽��ϴ�");
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
