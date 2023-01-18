#pragma once
#include "WiggleActor.h"
#include <vector>
#include <map>

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

	void Congratulations(void(*_EndFunction)(void));

	inline bool IsProgress() const
	{
		return IsProgressValue;
	}

protected:
	void Start() override;
	void Update(float _DT) override;

private:
	void(*EndFunction)(void) = nullptr;
	GameEngineRender* CongratulationRender = nullptr;

	std::vector<GameEngineRender*> vecCircleRenders;
	bool IsProgressValue = false;

	float ProgressTime = 0.0f;

};