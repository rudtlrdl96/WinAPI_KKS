#pragma once
#include "WiggleActor.h"

class FadeUI : public WiggleActor
{
public:
	FadeUI();
	~FadeUI();

	void Start() override;
	void Render() override;

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

protected:

private:
	GameEngineImage* FadeImage = nullptr;
};