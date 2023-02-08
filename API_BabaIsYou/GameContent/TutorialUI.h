#pragma once
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>

class GameEngineRender;
class TutorialUI : public GameEngineActor
{
public:
	TutorialUI();
	~TutorialUI();

	TutorialUI(const TutorialUI& _Other) = delete;
	TutorialUI(TutorialUI&& _Other) noexcept = delete;
	TutorialUI& operator=(const TutorialUI& _Other) = delete;
	TutorialUI& operator=(TutorialUI&& _Other) noexcept = delete;

	void SetTutorialImage(const std::string_view& _ImageName, float4 _Scale);

protected:
	void Start() override;

private:
	GameEngineRender* TutorialRender = nullptr;
};