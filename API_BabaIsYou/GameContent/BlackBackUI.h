#pragma once
#include "WiggleActor.h"
 
class BlackBackUI : public WiggleActor
{
public:
	BlackBackUI();
	~BlackBackUI();

	void Start() override;

	BlackBackUI(const BlackBackUI& _Other) = delete;
	BlackBackUI(BlackBackUI&& _Other) noexcept = delete;
	BlackBackUI& operator=(const BlackBackUI& _Other) = delete;
	BlackBackUI& operator=(BlackBackUI&& _Other) noexcept = delete;

protected:

private:
};