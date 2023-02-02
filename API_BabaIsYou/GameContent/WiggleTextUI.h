#pragma once
#include "WiggleActor.h"

class WiggleTextUI : public WiggleActor
{
public:
	WiggleTextUI();
	~WiggleTextUI();

	WiggleTextUI(const WiggleTextUI& _Other) = delete;
	WiggleTextUI(WiggleTextUI&& _Other) noexcept = delete;
	WiggleTextUI& operator=(const WiggleTextUI& _Other) = delete;
	WiggleTextUI& operator=(WiggleTextUI&& _Other) noexcept = delete;

	void SetText(char _Text);
	void SetScale(const float4& _Scale);
	void Init(int _Order);
protected:

private:
	int Order = 0;
};