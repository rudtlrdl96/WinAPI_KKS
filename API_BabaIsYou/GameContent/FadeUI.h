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
class FadeUI : public GameEngineActor
{
public:
	FadeUI();
	~FadeUI();

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

	void Fade(FADE_STATE _State, void (*_Func)(void) = nullptr);

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
	

	float DelayTime = 0.0f;
	float ProgressTime = 0.0f;

	FADE_STATE State = FADE_STATE::NONE;
};