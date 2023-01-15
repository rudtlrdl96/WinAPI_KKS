#pragma once
#include "WiggleActor.h"

class GrayBackUI : public WiggleActor
{
public:
	GrayBackUI();
	~GrayBackUI();

	void Start() override;

	GrayBackUI(const GrayBackUI& _Other) = delete;
	GrayBackUI(GrayBackUI&& _Other) noexcept = delete;
	GrayBackUI& operator=(const GrayBackUI& _Other) = delete;
	GrayBackUI& operator=(GrayBackUI&& _Other) noexcept = delete;

protected:

private:
};