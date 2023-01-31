#pragma once
#include <GameEngineCore/GameEngineActor.h>

class GameEngineRender;
class TextUI : public GameEngineActor
{
public:
	TextUI();
	~TextUI();

	TextUI(const TextUI& _Other) = delete;
	TextUI(TextUI&& _Other) noexcept = delete;
	TextUI& operator=(const TextUI& _Other) = delete;
	TextUI& operator=(TextUI&& _Other) noexcept = delete;

	void SetText(char _Text);
	void SetScale(const float4& _Scale);
protected:
	void Start() override;
private:
	GameEngineRender* TextRender = nullptr;
};