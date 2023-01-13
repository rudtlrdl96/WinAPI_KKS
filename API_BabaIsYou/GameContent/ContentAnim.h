#pragma once

class GameEngineRender;
class ContentAnim
{
public:
	ContentAnim(GameEngineRender* _Render, int _StartIdx, int _EndIdx);
	~ContentAnim();

	void UpdateAnim(float _DT);

	ContentAnim(const ContentAnim& _Other) = delete;
	ContentAnim(ContentAnim&& _Other) noexcept = delete;
	ContentAnim& operator=(const ContentAnim& _Other) = delete;
	ContentAnim& operator=(ContentAnim&& _Other) noexcept = delete;

	void SetProgress(float _Progress)
	{
		ProgressTime = _Progress;
	}

	void ActiveFilp()
	{
		Flip = true;
	}

	void DisableFilp()
	{
		Flip = false;
	}
		
	void ActiveWiggle()
	{
		Wiggle = true;
	}

	void DisableWiggle()
	{
		Wiggle = false;
	}

protected:

private:
	GameEngineRender* Render = nullptr;

	int AnimIndex = 0;
	int AnimStartIndex = 0;
	int AnimEndIndex = 0;

	float ProgressTime = 0.0f;

	bool Flip = false;
	bool Wiggle = false;
};