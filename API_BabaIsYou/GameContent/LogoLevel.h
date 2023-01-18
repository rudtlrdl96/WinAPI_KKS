#pragma once
#include <GameEngineCore/GameEngineLevel.h>

class FadeUI;
class LogoLevel : public GameEngineLevel
{
public:
	LogoLevel();
	~LogoLevel();

	LogoLevel(const LogoLevel& _Other) = delete;
	LogoLevel(LogoLevel&& _Other) noexcept = delete;
	LogoLevel& operator=(const LogoLevel& _Other) = delete;
	LogoLevel& operator=(LogoLevel&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void Update(float _DT) override;
	void LevelChangeStart(GameEngineLevel* _NextLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}
private:
	FadeUI* LogoFadeActor = nullptr;
};