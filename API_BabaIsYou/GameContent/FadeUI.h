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

	FadeUI(const FadeUI& _Other) = delete;
	FadeUI(FadeUI&& _Other) noexcept = delete;
	FadeUI& operator=(const FadeUI& _Other) = delete;
	FadeUI& operator=(FadeUI&& _Other) noexcept = delete;

	void SetState(FADE_STATE _State);
	static void ActiveFade(FADE_STATE _State, GameEngineLevel* _ParentLevel, void (*_Func)(void));
protected:
	void Start() override;
	void Update(float _Time) override;

private:
	static std::map<GameEngineLevel*, FadeUI*> mapFades;
	static void (*FuncPtr)(void) ;

	GameEngineRender* FadeRender = nullptr;
	GameEngineRender* BoxRender = nullptr;
	
	float DelayTime = 0.0f;
	float FadeTime = 0.5f;
	float ProgressTime = 0.0f;

	FADE_STATE State = FADE_STATE::NONE;
};