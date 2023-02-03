#pragma once
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

class GameEngineRender;
class ParticleActor : public GameEngineActor
{
public:
	ParticleActor();
	~ParticleActor();

	ParticleActor(const ParticleActor& _Other) = delete;
	ParticleActor(ParticleActor&& _Other) noexcept = delete;
	ParticleActor& operator=(const ParticleActor& _Other) = delete;
	ParticleActor& operator=(ParticleActor&& _Other) noexcept = delete;

	void StartParticle(const std::string_view& _ParticleName, PARTICLE_COLOR _Color, float4 _Size);
	void ParticleReturn();

	std::string ConvertParticleColorName(PARTICLE_COLOR _Color);

	inline bool IsParticleEnd() const
	{
		return IsParticleEndValue;
	}


protected:
	void Start() override;
	void Update(float _DT) override;
	
	void LevelChangeEnd(GameEngineLevel* _PrevLevel) override
	{
		ParticleReturn();
	}

private:
	bool IsParticleEndValue = true;
	GameEngineRender* ParticleRender = nullptr;

};