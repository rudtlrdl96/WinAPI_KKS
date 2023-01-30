#pragma once
#include "StringUI.h"

class WiggleStringUI : public StringUI
{
public:
	WiggleStringUI();
	~WiggleStringUI();

	WiggleStringUI(const WiggleStringUI& _Other) = delete;
	WiggleStringUI(WiggleStringUI&& _Other) noexcept = delete;
	WiggleStringUI& operator=(const WiggleStringUI& _Other) = delete;
	WiggleStringUI& operator=(WiggleStringUI&& _Other) noexcept = delete;

protected:
	GameEngineActor* CreateTextActor(char _Text) override;

private:

};