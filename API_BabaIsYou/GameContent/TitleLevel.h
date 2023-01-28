#pragma once
#include <GameEngineCore/GameEngineLevel.h>

class FadeUI;
class ButtonUI;
class TitleLevel : public GameEngineLevel
{
public:
	TitleLevel();
	~TitleLevel();

	TitleLevel(const TitleLevel& _Other) = delete;
	TitleLevel(TitleLevel&& _Other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _Other) = delete;
	TitleLevel& operator=(TitleLevel&& _Other) noexcept = delete;


protected:
	void Loading() override;
	void Update(float _DT) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}
private:
	FadeUI* TitleFadeActor = nullptr;

	ButtonUI* GameStartButton = nullptr;
	ButtonUI* MapToolButton = nullptr;
	ButtonUI* ExitButton = nullptr;
};