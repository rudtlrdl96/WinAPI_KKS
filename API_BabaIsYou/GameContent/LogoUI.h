#pragma once
#include "WiggleActor.h"

class LogoUI : public WiggleActor
{
public:
	LogoUI();
	~LogoUI();

	LogoUI(const LogoUI& _Other) = delete;
	LogoUI(LogoUI&& _Other) noexcept = delete;
	LogoUI& operator=(const LogoUI& _Other) = delete;
	LogoUI& operator=(LogoUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update() override;
	void Render() override;

private:
	GameEngineImage* LogoImage = nullptr;
};