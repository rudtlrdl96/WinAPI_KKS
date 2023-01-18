#pragma once
#include <vector>
#include <string_view>
#include "ContentMath.h"

class GameEngineRender;
class WiggleRender
{
public:
	WiggleRender(GameEngineRender* _WiggleRender, int _StartIndex, int _AnimLength, int _BitmapInterval);
	~WiggleRender();

	void SetRenderIndex();
	void Wiggle();
	void PrevAnim();
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

	inline void SetStartIndex(int _Index)
	{
		StartIndex = _Index;
	}

	inline void SetAnimLength(int _Length)
	{
		AnimLength = _Length;
	}

	inline void SetTileIndex(int _Index)
	{
		CurTileIndex = _Index;
		SetRenderIndex();
	}

	void SetDirInterval(size_t _DirInterval);
	void SetAnimDir(const int2& _Dir);

	void RenderOn();
	void RenderOff();

protected:

private:
	int CurIndex = 0;
	int CurAnim = 0;
	int CurWiggle = 0;
	int CurTileIndex = 0;

	int StartIndex = 0;
	int AnimLength = 1;
	int BitmapInterval = 0;

	size_t DirIndex = 0;
	size_t DirInterval = 0;

	bool IsWiggle = false;
	GameEngineRender* Render = nullptr;
};