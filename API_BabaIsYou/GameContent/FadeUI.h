#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>
#include <map>

enum class FADE_STATE
{
	NONE,
	FADEIN,
	FADEOUT
};

class GameEngineRender;
class ContentAnim;
class FadeUI : public GameEngineActor
{
public:
	FadeUI();
	~FadeUI();

	void Start() override;
	void Update(float _Time) override;

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

	static void FadeIn(GameEngineLevel* _ParentLevel, void (*_Func)(void));
	static void FadeOut(GameEngineLevel* _ParentLevel, void (*_Func)(void));

protected:

private:
	static std::map<GameEngineLevel*, FadeUI*> mapFades;
	static void (*FuncPtr)(void) ;

	GameEngineRender* FadeRender = nullptr;
	
	float DelayTime = 0.0f;
	float FadeTime = 0.5f;
	float ProgressTime = 0.0f;

	FADE_STATE State = FADE_STATE::NONE;
};