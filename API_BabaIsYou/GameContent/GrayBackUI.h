#pragma once
#include "WiggleActor.h"

class GameEngineRender;
class GrayBackUI : public WiggleActor
{
public:
	GrayBackUI();
	~GrayBackUI();

	GrayBackUI(const GrayBackUI& _Other) = delete;
	GrayBackUI(GrayBackUI&& _Other) noexcept = delete;
	GrayBackUI& operator=(const GrayBackUI& _Other) = delete;
	GrayBackUI& operator=(GrayBackUI&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	GameEngineRender* BackRender = nullptr;
};