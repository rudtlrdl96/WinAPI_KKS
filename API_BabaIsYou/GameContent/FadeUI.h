#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>

class GameEngineImage;
class FadeUI : public GameEngineActor
{
private:
	enum FADE_STATE
	{
		NONE	= 0,
		FADEIN	= 1,
		FADEOUT	= 2,
	};

	class LerpFadeData
	{
	public:
		GameEngineImage* Image = nullptr;

		float4 Start = float4::Zero;
		float4 Dest = float4::Zero;
		float4 Scale = float4::Zero;
		float4 BitSize = float4::Zero;
	};
public:
	FadeUI();
	~FadeUI();

	void Start() override;
	void Update() override;
	void Render() override;

	static void FadeIn();
	static void FadeOut();
	static void Reset();

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

protected:

private:
	static FADE_STATE State;
	static float FadeScale;

	GameEngineImage* CircleImage = nullptr;
	GameEngineImage* BoxImage = nullptr;

	std::vector<LerpFadeData> vecFadePos;

	void PushFade(GameEngineImage* _Image, const float4& _Start, const float4& _Interval, const float4& _BitmapSize, const float4& _Scale);
};