#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "StringUI.h"

class WiggleTextUI;
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
	void CreateTextActor(char _Text) override;
private:
};