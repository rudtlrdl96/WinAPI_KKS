#pragma once
#include "WiggleActor.h"
#include <vector>

class GameEngineRender;
class CongratulationsUI : public WiggleActor
{
public:
	CongratulationsUI();
	~CongratulationsUI();

	CongratulationsUI(const CongratulationsUI& _Other) = delete;
	CongratulationsUI(CongratulationsUI&& _Other) noexcept = delete;
	CongratulationsUI& operator=(const CongratulationsUI& _Other) = delete;
	CongratulationsUI& operator=(CongratulationsUI&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	std::vector<GameEngineRender*> vecCircleRenders;
};