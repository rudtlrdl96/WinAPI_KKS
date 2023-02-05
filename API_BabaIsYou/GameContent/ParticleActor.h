#pragma once
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

class GameEngineRender;
class ParticleSystem;
class ParticleActor : public GameEngineActor
{
	friend ParticleSystem;
public:
	ParticleActor();
	~ParticleActor();

	ParticleActor(const ParticleActor& _Other) = delete;
	ParticleActor(ParticleActor&& _Other) noexcept = delete;
	ParticleActor& operator=(const ParticleActor& _Other) = delete;
	ParticleActor& operator=(ParticleActor&& _Other) noexcept = delete;

	void MoveParticle(float4 _Dir, float _Speed);
	void ParticleReturn();

	std::string ConvertParticleColorName(PARTICLE_COLOR _Color);

	inline bool IsParticleEnd() const
	{
		return IsParticleEndValue;
	}


protected:
	void Start() override;
	void Update(float _DT) override;

private:
	bool IsParticleEndValue = true;
	bool IsUseParticle = false;

	GameEngineRender* ParticleRender = nullptr;

	bool IsMoveParticle = false;
	float4 ParticleMoveDir = float4::Zero;
	float ParticelMoveSpeed = 0.0f;

	void StartParticle(const std::string_view& _ParticleName, PARTICLE_COLOR _Color, float4 _Size);
};