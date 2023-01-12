#pragma once
#include "WiggleActor.h"

class BackgroundBoxUI : public WiggleActor
{
public:
	BackgroundBoxUI();
	~BackgroundBoxUI();

	void Start() override;
	void Render(float _DT) override;

	BackgroundBoxUI(const BackgroundBoxUI& _Other) = delete;
	BackgroundBoxUI(BackgroundBoxUI&& _Other) noexcept = delete;
	BackgroundBoxUI& operator=(const BackgroundBoxUI& _Other) = delete;
	BackgroundBoxUI& operator=(BackgroundBoxUI&& _Other) noexcept = delete;

protected:

private:
	GameEngineImage* BoxImage = nullptr;
};