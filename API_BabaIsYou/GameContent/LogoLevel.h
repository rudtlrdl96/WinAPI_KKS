#pragma once
#include <GameEngineCore/GameEngineLevel.h>

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
	void Update() override;
private:
};