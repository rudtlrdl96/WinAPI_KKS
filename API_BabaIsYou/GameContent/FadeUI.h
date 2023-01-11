#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>

class GameEngineImage;
class FadeUI : public GameEngineActor
{
private:
	enum FADE_STATE
	{
		NONE	= 0,
		FADEIN	= 1,
		FADEOUT	= 2,
	};
public:
	FadeUI();
	~FadeUI();

	void Start() override;
	void Update() override;
	void Render() override;

	static void FadeIn();
	static void FadeOut();
	static void Reset();

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

protected:

private:
	static FADE_STATE State;
	static float FadeScale;

	GameEngineImage* CircleImage = nullptr;
	GameEngineImage* BoxImage = nullptr;

	std::vector<std::pair<float4, float4>> vecBoxPos;
	std::vector<std::pair<float4, float4>> vecCirclePos;
};