#include "ParticleActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"

const std::string MoveAnimName = "Move";
const std::string GlitteringAnimName = "Glittering";
const std::string ExplosionAnimName = "Explosion";
const std::string SmokeAnimName = "Smoke";

ParticleActor::ParticleActor()
{
}

ParticleActor::~ParticleActor()
{
}

void ParticleActor::StartParticle(const std::string_view& _ParticleName, PARTICLE_COLOR _Color, float4 _Size)
{
}

void ParticleActor::ParticleReturn()
{
	if (true == IsUpdate())
	{
		Off();
		// Return...
	}
}

std::string ParticleActor::ConvertParticleColorName(PARTICLE_COLOR _Color)
{
	return "_White";
}

void ParticleActor::Start()
{
	ParticleRender = CreateRender("Particle.BMP", RENDER_ORDER::PARTICLE);

	for (int i = 0; i < static_cast<int>(PARTICLE_COLOR::COUNT); i++)
	{
		int ColorBitmapIndex = i * 28;

		ParticleRender->CreateAnimation(
			{.AnimationName = MoveAnimName + ConvertParticleColorName(static_cast<PARTICLE_COLOR>(i)),
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 0,
			.End = ColorBitmapIndex + 4,
			.InterTime = 0.1f,
			.Loop = false});

		ParticleRender->CreateAnimation(
			{ .AnimationName = GlitteringAnimName + ConvertParticleColorName(static_cast<PARTICLE_COLOR>(i)),
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 7,
			.End = ColorBitmapIndex + 12,
			.InterTime = 0.1f,
			.Loop = false });

		ParticleRender->CreateAnimation(
			{ .AnimationName = ExplosionAnimName + ConvertParticleColorName(static_cast<PARTICLE_COLOR>(i)),
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 14,
			.End = ColorBitmapIndex + 18,
			.InterTime = 0.1f,
			.Loop = false });

		ParticleRender->CreateAnimation(
			{ .AnimationName = SmokeAnimName + ConvertParticleColorName(static_cast<PARTICLE_COLOR>(i)),
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 21,
			.End = ColorBitmapIndex + 27,
			.InterTime = 0.1f,
			.Loop = false });
	}
}

void ParticleActor::Update(float _DT)
{
	if (nullptr == ParticleRender)
	{
		MsgAssert("파티클 랜더러가 삭제되었습니다");
		return;
	}

	IsParticleEndValue = ParticleRender->IsAnimationEnd();

	if (true == IsParticleEndValue)
	{
		ParticleReturn();
	}
}