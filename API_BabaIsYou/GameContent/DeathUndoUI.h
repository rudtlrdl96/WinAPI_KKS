#pragma once
#include <GameEngineCore/GameEngineActor.h>

class DeathUndoUI : public GameEngineActor
{
public:
	DeathUndoUI();
	~DeathUndoUI();

	DeathUndoUI(const DeathUndoUI& _Other) = delete;
	DeathUndoUI(DeathUndoUI&& _Other) noexcept = delete;
	DeathUndoUI& operator=(const DeathUndoUI& _Other) = delete;
	DeathUndoUI& operator=(DeathUndoUI&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};