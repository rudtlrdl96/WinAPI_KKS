#pragma once
#include "WiggleActor.h"
 
class BlackBoxUI : public WiggleActor
{
public:
	BlackBoxUI();
	~BlackBoxUI();

	void Start() override;

	BlackBoxUI(const BlackBoxUI& _Other) = delete;
	BlackBoxUI(BlackBoxUI&& _Other) noexcept = delete;
	BlackBoxUI& operator=(const BlackBoxUI& _Other) = delete;
	BlackBoxUI& operator=(BlackBoxUI&& _Other) noexcept = delete;

protected:

private:
};