#pragma once
#include <GameEngineCore/GameEngineActor.h>

class DeathRestartUI : public GameEngineActor
{
public:
	DeathRestartUI();
	~DeathRestartUI();

	DeathRestartUI(const DeathRestartUI& _Other) = delete;
	DeathRestartUI(DeathRestartUI&& _Other) noexcept = delete;
	DeathRestartUI& operator=(const DeathRestartUI& _Other) = delete;
	DeathRestartUI& operator=(DeathRestartUI&& _Other) noexcept = delete;

protected:
	void Start() override;
private:

};