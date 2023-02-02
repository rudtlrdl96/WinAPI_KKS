#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

class GameEngineRender;
class TextUI : public GameEngineActor
{
public:
	TextUI();
	~TextUI();

	TextUI(const TextUI& _Other) = delete;
	TextUI(TextUI&& _Other) noexcept = delete;
	TextUI& operator=(const TextUI& _Other) = delete;
	TextUI& operator=(TextUI&& _Other) noexcept = delete;

	virtual void Init(int _Order);
	virtual void SetText(char _Text);
	virtual void SetScale(const float4& _Scale);

	inline void SetTextColor(TEXT_COLOR _Color)
	{
		TextColor = _Color;
	}

	void LerpSize(float4 _Start, float4 _End, float _Time, bool _IsEndOff);
	void ShakeText(float4 _Dir, float _Time);
	void ResetShake();
protected:
	TEXT_COLOR TextColor = TEXT_COLOR::WHITE;

	void Update(float _DT) override;
private:
	GameEngineRender* TextRender = nullptr;
	int Order = 0;
	char CurText;

	bool IsShakeValue = false;

	float4 CurPos = float4::Zero;
	float4 ShakeStartPos = float4::Zero;
	float4 ShakeEndPos = float4::Zero;

	float LerpShakeRatio = 0.0f;
	float ShakeTime = 0.0f;

	bool IsSizeLerp = false;
	bool IsSizeLerpEndOff = false;

	float4 LerpStartSize = float4::Zero;
	float4 LerpEndSize = float4::Zero;

	float LerpSizeRatio = 0.0f;
	float SizeTime = 0.0f;
};