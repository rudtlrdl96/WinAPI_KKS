#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>

class TextUI;
class StringUI : public GameEngineActor
{
public:
	StringUI();
	~StringUI();

	StringUI(const StringUI& _Other) = delete;
	StringUI(StringUI&& _Other) noexcept = delete;
	StringUI& operator=(const StringUI& _Other) = delete;
	StringUI& operator=(StringUI&& _Other) noexcept = delete;

	virtual void WriteText(const std::string_view& _Text);
	
	inline void SetFontSize(const float4& _FontSize)
	{
		FontSizeValue = _FontSize;
	}

	inline float4 GetFontSize() const
	{
		return FontSizeValue;
	}

	inline void SetFontInterval(const float4& _FontInterval)
	{
		FontInterval = _FontInterval;
	}

	inline float4 GetFontInterval()
	{
		return FontInterval;
	}

	void StringOn();
	void StringOff();

protected:

private:
	std::vector<TextUI*> vecTextActors;
	int ActiveActorCount = 0;

	float4 FontSizeValue = float4::Zero;
	float4 FontInterval = float4::Zero;

	virtual TextUI* CreateTextActor(char _Text);
};