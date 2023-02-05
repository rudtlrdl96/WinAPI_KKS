#pragma once
#include <vector>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

class ParticleActor;
class ParticleSystem : public GameEngineActor
{
public:
	ParticleSystem();
	~ParticleSystem();

	ParticleSystem(const ParticleSystem& _Other) = delete;
	ParticleSystem(ParticleSystem&& _Other) noexcept = delete;
	ParticleSystem& operator=(const ParticleSystem& _Other) = delete;
	ParticleSystem& operator=(ParticleSystem&& _Other) noexcept = delete;

	ParticleActor* UseParticle(std::string _AnimName, PARTICLE_COLOR _Color, float4 _Pos, float4 _Size);
	void ReturnItem(ParticleActor* _Particle);

	static ParticleSystem* GetLevelParticleSystem();

protected:
	void Start() override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _PrevLevel) override;

private:
	static ParticleSystem* CurLevelParticleSystem;

	std::vector<ParticleActor*> vecAllParticles;
	std::vector<ParticleActor*> vecWaitParticles;

	void CreateParticle();

};