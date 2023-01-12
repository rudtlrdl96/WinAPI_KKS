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

public:
	FadeUI();
	~FadeUI();

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

protected:

private:
};