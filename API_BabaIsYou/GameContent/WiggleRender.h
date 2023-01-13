#pragma once
#include <vector>
#include <string_view>

class GameEngineRender;
class WiggleRender
{
public:
	WiggleRender(GameEngineRender* _WiggleRender, int _StartIndex, int _AnimLength, int _Interval);
	~WiggleRender();

	void SetRenderIndex();
	void Wiggle();
	void NextAnim();

	WiggleRender(const WiggleRender& _Other) = delete;
	WiggleRender(WiggleRender&& _Other) noexcept = delete;
	WiggleRender& operator=(const WiggleRender& _Other) = delete;
	WiggleRender& operator=(WiggleRender&& _Other) noexcept = delete;

	inline void ActiveWiggle()
	{
		IsWiggle = true;
	}

	inline void DisableWiggle()
	{
		IsWiggle = false;
	}

protected:

private:
	int CurIndex = 0;
	int CurAnim = 0;
	int CurWiggle = 0;

	int StartIndex = 0;
	int AnimLength = 1;
	int Interval = 0;

	bool IsWiggle = false;
	GameEngineRender* Render = nullptr;
};