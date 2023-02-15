#pragma once
#include <vector>
#include <string_view>
#include "ContentMath.h"

class GameEngineRender;
class WiggleRender
{
public:
	WiggleRender(int _StartIndex, int _AnimLength, int _BitmapInterval);
	~WiggleRender();

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

	inline void ActiveText()
	{
		IsText = true;
	}

	inline void DisableText()
	{
		IsText = false;
	}
	
	inline void ActiveTile()
	{
		IsTile = true;
	}

	inline void DisableTile()
	{
		IsTile = false;
	}

	inline void ActiveDir()
	{
		IsDir = true;
	}

	inline void DisableDir()
	{
		IsDir = false;
	}

	inline void ActiveAnimation()
	{
		IsAnimation = true;
	}

	inline void DisableAnimation()
	{
		IsAnimation = false;
	}
	
	void Wiggle();
	void Reset();

	void CalculationRenderIndex();

	void SetRender(GameEngineRender* _WiggleRender);
	void SetOrder(int _Order);
	void SetStartIndex(int _Index);
	void SetAnimLength(int _Length);
	void SetCompletionText(bool _IsCompletion);
	void SetTileIndex(int _Index);
	void SetDirInterval(size_t _DirInterval);
	void SetAnimDir(const int2& _Dir);
	void SetScale(const float4& _Scale);

	void PrevAnim();
	void NextAnim();
	void ResetAnim();

	void RenderOn();
	void RenderOff();
	void CameraEffectOff();

protected:

private:
	int CurIndex = 0;
	int CurAnimationIndex = 0;
	int CurWiggleIndex = 0;
	int CurTextIndex = 0;
	int CurTileIndex = 0;

	int StartIndex = 0;
	int AnimLength = 1;
	int BitmapInterval = 0;

	size_t DirIndex = 0;
	size_t DirInterval = 0;

	bool IsWiggle = false;
	bool IsTile = false;
	bool IsDir = false;
	bool IsText = false;
	bool IsAnimation = false;

	GameEngineRender* Render = nullptr;
};