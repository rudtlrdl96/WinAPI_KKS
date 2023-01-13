#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <vector>

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
	static std::vector<FadeUI*> vecFades;
	static void (*FuncPtr)(void) ;

	ContentAnim* FadeRender = nullptr;
	
	float ProgressTime = 0.0f;
};