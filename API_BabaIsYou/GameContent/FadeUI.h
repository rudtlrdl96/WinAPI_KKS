#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>
#include <map>

enum class FADE_STATE
{
	NONE,
	FADEIN,
	FADEOUT
};

class GameEngineRender;
class ContentAnim;
class StringUI;
class FadeUI : public GameEngineActor
{
public:
	class FadeParameter
	{
	public:
		FADE_STATE State = FADE_STATE::NONE;
		void (*Func)(void) = nullptr; 
		float WaitTime = 0.0f;
		float DelayTime = 0.0f;

		std::string WriteTopText = "";
		float4 TopTextInterval = float4::Zero;
		float4 TopTextSize = float4::Zero;
		
		std::string WriteMiddleText = "";
		float4 MiddleTextInterval = float4::Zero;
		float4 MiddleTextSize = float4::Zero;
	};

public:
	FadeUI();
	~FadeUI();

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

	void Fade(const FadeParameter& _FadeParameter);

	inline bool IsProgress() const
	{
		return IsProgressValue;
	}

protected:
	void Start() override;
	void Update(float _Time) override;

private:
	void (*EndFunction)(void) = nullptr;
	bool IsProgressValue = false;

	GameEngineRender* FadeRender = nullptr;
	GameEngineRender* BoxRender = nullptr;
	
	StringUI* TopStringUIRender = nullptr;
	StringUI* MiddleStringUIRender = nullptr;

	float WaitTime = 0.0f;
	float DelayTime = 0.0f;
	float ProgressWaitTime = 0.0f;
	float ProgressDelayTime = 0.0f;

	float FadeRatio = 0.0f;

	bool IsWait = false;
	bool IsDelay = false;

	FADE_STATE State = FADE_STATE::NONE;
};