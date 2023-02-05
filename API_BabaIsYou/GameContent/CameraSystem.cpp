#include "CameraSystem.h"
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentRand.h"

CameraSystem* CameraSystem::CurLevelParticleSystem = nullptr;

CameraSystem::CameraSystem()
{
}

CameraSystem::~CameraSystem()
{
}

CameraSystem* CameraSystem::GetLevelCameraSystem()
{
	if (nullptr == CurLevelParticleSystem)
	{
		MsgAssert("카메라 시스템을 생성하지않고 사용하려 했습니다.");
	}

	return CurLevelParticleSystem;
}

void CameraSystem::CameraLerp(float4 _Start, float4 _End, float _Speed)
{
	LerpStart = _Start;
	LerpEnd = _End;
	LerpProgress = 0.0f;
	LerpSpeed = _Speed;
	IsLerpValue = true;
}

void CameraSystem::CameraShake(float _ShakeDis, float _ShakeSpeed, int _ShakeCount)
{
	ShakePos = float4::Zero;
	ShakeDis = _ShakeDis;
	ShakeSpeed = _ShakeSpeed;
	ShakeCount = _ShakeCount;
	IsShakeValue = true;
	SetShakeValue();
	ShakeProgress = 0.0f;
}

void CameraSystem::Update(float _DT)
{
	if (true == IsLerpValue)
	{
		CalculationLerp(_DT);
	}

	if (true == IsShakeValue)
	{
		CalculationShake(_DT);
	}

	GetLevel()->SetCameraPos(CameraPos + ShakePos);
}

void CameraSystem::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr != CurLevelParticleSystem)
	{
		MsgAssert("카메라 시스템을 중복 생성했습니다.");
		return;
	}

	CurLevelParticleSystem = this;
}

void CameraSystem::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	if (nullptr == CurLevelParticleSystem)
	{
		MsgAssert("카메라 시스템을 중복 생성했습니다.");
		return;
	}

	CurLevelParticleSystem = nullptr;
}

void CameraSystem::SetShakeValue()
{
	ShakeStart = ShakePos;
	float RandRot = ContentRand::RandFloat(0.0f, GameEngineMath::PIE2);

	if (1 >= ShakeCount)
	{
		ShakeEnd = float4::Zero;
	}
	else
	{
		ShakeEnd.x = std::cos(RandRot);
		ShakeEnd.y = std::sin(RandRot);
		ShakeEnd *= ShakeDis;
	}

	ShakeProgress -= 1.0f;
}

void CameraSystem::CalculationLerp(float _DT)
{
	LerpProgress += _DT * LerpSpeed;
	CameraPos = float4::LerpClamp(LerpStart, LerpEnd, LerpProgress);

	if (1.0f <= LerpProgress)
	{
		LerpProgress = 0.0f;
		IsLerpValue = false;
	}
}

void CameraSystem::CalculationShake(float _DT)
{
	ShakeProgress += _DT * ShakeSpeed;
	ShakePos = float4::LerpClamp(ShakeStart, ShakeEnd, ShakeProgress);

	if (1.0f <= ShakeProgress)
	{
		--ShakeCount;

		if (0 < ShakeCount)
		{
			SetShakeValue();
		}
		else
		{
			IsShakeValue = false;
			ShakePos = float4::Zero;
			ShakeProgress = 0.0f;
		}
	}
}