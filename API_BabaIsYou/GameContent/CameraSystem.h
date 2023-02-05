#pragma once
#include <GameEngineCore/GameEngineActor.h>

class CameraSystem : public GameEngineActor
{
public:
	CameraSystem();
	~CameraSystem();

	CameraSystem(const CameraSystem& _Other) = delete;
	CameraSystem(CameraSystem&& _Other) noexcept = delete;
	CameraSystem& operator=(const CameraSystem& _Other) = delete;
	CameraSystem& operator=(CameraSystem&& _Other) noexcept = delete;

	static CameraSystem* GetLevelCameraSystem();

	inline void SetCameraPos(const float4& _Pos)
	{
		CameraPos = _Pos;
	}

	inline float4 GetCameraPos() const
	{
		return CameraPos;
	}

	inline bool IsCameraLerp() const
	{
		return IsLerpValue;
	}

	inline bool IsCameraShake() const
	{
		return IsShakeValue;
	}

	void CameraLerp(float4 _Start, float4 _End, float _Time);
	void CameraShake(float _ShakeDis, float _ShakeSpeed, int _ShakeCount);

protected:
	void Update(float _DT) override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _PrevLevel) override;

private:
	static CameraSystem* CurLevelParticleSystem;

	bool IsShakeValue = false;
	bool IsLerpValue = false;

	float4 CameraPos = float4::Zero;
	float4 LerpStart = float4::Zero;
	float4 LerpEnd = float4::Zero;

	float4 ShakePos = float4::Zero;
	float4 ShakeStart = float4::Zero;
	float4 ShakeEnd = float4::Zero;

	float LerpProgress = 0.0f;
	float LerpSpeed = 0.0f;

	float ShakeProgress = 0.0f;
	float ShakeSpeed = 0.0f;
	float ShakeDis = 0.0f;

	int ShakeCount = 0;

	void SetShakeValue();
	void CalculationShake(float _DT);
	void CalculationLerp(float _DT);
};