#pragma once
#include "TextUI.h"

class WiggleRender;
class WiggleTextUI : public TextUI
{
public:
	WiggleTextUI();
	~WiggleTextUI();

	WiggleTextUI(const WiggleTextUI& _Other) = delete;
	WiggleTextUI(WiggleTextUI&& _Other) noexcept = delete;
	WiggleTextUI& operator=(const WiggleTextUI& _Other) = delete;
	WiggleTextUI& operator=(WiggleTextUI&& _Other) noexcept = delete;

	void Init(int _Order) override;
	void SetText(char _Text) override;
	void SetScale(const float4& _Scale) override;

protected:
	void Update(float _DT) override;
private:
	WiggleRender* Render = nullptr;
	float ProgressTime = 0.0f;
};