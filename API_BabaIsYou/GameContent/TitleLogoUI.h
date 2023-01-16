#pragma once
#include "WiggleActor.h"

class TitleLogoUI : public WiggleActor
{
public:
	TitleLogoUI();
	~TitleLogoUI();

	TitleLogoUI(const TitleLogoUI& _Other) = delete;
	TitleLogoUI(TitleLogoUI&& _Other) noexcept = delete;
	TitleLogoUI& operator=(const TitleLogoUI& _Other) = delete;
	TitleLogoUI& operator=(TitleLogoUI&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};