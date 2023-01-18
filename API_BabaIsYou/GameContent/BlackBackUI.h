#pragma once
#include "WiggleActor.h"
 
class GameEngineRender;
class BlackBackUI : public WiggleActor
{
public:
	BlackBackUI();
	~BlackBackUI();

	BlackBackUI(const BlackBackUI& _Other) = delete;
	BlackBackUI(BlackBackUI&& _Other) noexcept = delete;
	BlackBackUI& operator=(const BlackBackUI& _Other) = delete;
	BlackBackUI& operator=(BlackBackUI&& _Other) noexcept = delete;

	void SetScale(const float4& _Scale);

protected:
	void Start() override;

private:
	GameEngineRender* BackRender = nullptr; 
};