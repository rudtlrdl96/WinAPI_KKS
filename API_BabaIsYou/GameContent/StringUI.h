#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>

class StringUI : public GameEngineActor
{
public:
	StringUI();
	~StringUI();

	StringUI(const StringUI& _Other) = delete;
	StringUI(StringUI&& _Other) noexcept = delete;
	StringUI& operator=(const StringUI& _Other) = delete;
	StringUI& operator=(StringUI&& _Other) noexcept = delete;

	void WriteText(const std::string_view& _Text);
	
	inline void SetFontSize(const float4& _FontSize)
	{
		FontSizeValue = _FontSize;
	}

	inline float4 GetFontSize() const
	{
		return FontSizeValue;
	}

protected:
	virtual GameEngineActor* CreateTextActor(char _Text);

	std::vector<GameEngineActor*> vecTextActors;
private:
	float4 FontSizeValue = float4::Zero;
};