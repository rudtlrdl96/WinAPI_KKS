#include "ParticleActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentEnum.h"
#include "ParticleSystem.h"

ParticleActor::ParticleActor()
{
}

ParticleActor::~ParticleActor()
{
}

void ParticleActor::StartParticle(const std::string_view& _ParticleName, PARTICLE_COLOR _Color, float4 _Size)
{
	ParticleRender->ChangeAnimation(_ParticleName.data() + ConvertParticleColorName(_Color), true);
	ParticleRender->SetScale(_Size);
	On();
}

void ParticleActor::ParticleReturn()
{
	if (true == IsUpdate())
	{
		Off();
		ParticleSystem::GetLevelParticleSystem()->ReturnItem(this);
	}
}

std::string ParticleActor::ConvertParticleColorName(PARTICLE_COLOR _Color)
{
	switch (_Color)
	{
	case PARTICLE_COLOR::WHITE:
		return "_WHITE";
	case PARTICLE_COLOR::BURGUNDY:
		return "_BURGUNDY";
	case PARTICLE_COLOR::GRAY:
		return "_GRAY";
	case PARTICLE_COLOR::JAVA:
		return "_JAVA";
	case PARTICLE_COLOR::ANZAC:
		return "_ANZAC";
	case PARTICLE_COLOR::SUSHI:
		return "_SUSHI";
	case PARTICLE_COLOR::CLAY:
		return "_CLAY";
	case PARTICLE_COLOR::MOCHA:
		return "_MOCHA";
	case PARTICLE_COLOR::FLAX:
		return "_FLAX";
	case PARTICLE_COLOR::ROYALBLUE:
		return "_ROYALBLUE";
	case PARTICLE_COLOR::BLACK:
		return "_BLACK";
	default:
		return "_WHITE";
	}
}

void ParticleActor::Start()
{
	ParticleRender = CreateRender("Particle.BMP", RENDER_ORDER::PARTICLE);

	std::string MoveAnimName = "Move";
	std::string GlitteringAnimName = "Glittering";
	std::string ExplosionAnimName = "Explosion";
	std::string SmokeAnimName = "Smoke";

	for (int i = 0; i < static_cast<int>(PARTICLE_COLOR::COUNT); i++)
	{
		int ColorBitmapIndex = i * 28;

		std::string ColorName = ConvertParticleColorName(static_cast<PARTICLE_COLOR>(i));
		MoveAnimName = "Move" + ColorName;
		ParticleRender->CreateAnimation({				
			.AnimationName = MoveAnimName,
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 0,
			.End = ColorBitmapIndex + 4,
			.InterTime = 0.1f,
			.Loop = false});

		GlitteringAnimName = "Glittering" + ColorName;
		ParticleRender->CreateAnimation({ 
			.AnimationName = GlitteringAnimName,
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 7,
			.End = ColorBitmapIndex + 12,
			.InterTime = 0.1f,
			.Loop = false });

		ExplosionAnimName = "Explosion" + ColorName;
		ParticleRender->CreateAnimation({ 
			.AnimationName = ExplosionAnimName,
			.ImageName = "Particle.BMP",
			.Start = ColorBitmapIndex + 14,
			.End = ColorBitmapIndex + 18,
			.InterTime = 0.1f,
			.Loop = false });

		SmokeAnimName = "Smoke" + ColorName;
		ParticleRender->CreateAnimation({ 
			.AnimationName = SmokeAnimName,
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