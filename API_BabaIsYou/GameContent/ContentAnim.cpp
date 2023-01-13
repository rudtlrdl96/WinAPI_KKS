#include "ContentAnim.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineCore/GameEngineRender.h>
#include "ContentConst.h"
#include "ContentMath.h"

ContentAnim::ContentAnim(GameEngineRender* _Render, int _StartIdx, int _EndIdx) :
	Render(_Render),
	AnimStartIndex(_StartIdx),
	AnimEndIndex(_EndIdx)
{
	if (nullptr == Render)
	{
		MsgAssert("nullptr Render를 이용해 ContentAnim을 생성하려 했습니다.");
	}
}

ContentAnim::~ContentAnim()
{
}

void ContentAnim::UpdateAnim(float _DT)
{
	if (nullptr == Render)
	{
		return;
	}

	Render->On();

	if (true == Flip)
	{
		ProgressTime -= _DT;
	}
	else
	{
		ProgressTime += _DT;
	}

	if (true == Wiggle)
	{
		if (1.0f <= ProgressTime)
		{
			if (1.0f  <= ProgressTime + ContentConst::WIGGLE_TIME)
			{
				ProgressTime -= ContentConst::WIGGLE_TIME;
				++AnimIndex;
			}

			if (AnimIndex >= AnimEndIndex)
			{
				AnimIndex = AnimEndIndex - 3;
			}
		}
		else
		{
			AnimIndex = Lerp(AnimStartIndex, AnimEndIndex, ProgressTime);
		}
	}
	else
	{
		if (0.0f > ProgressTime)
		{
			ProgressTime = 0.0f;
			Render->Off();
		}

		if (1.0f <= ProgressTime)
		{
			AnimIndex = AnimEndIndex - 1;
		}
		else
		{
			AnimIndex = Lerp(AnimStartIndex, AnimEndIndex, ProgressTime);
		}
	}

	Render->SetFrame(AnimIndex);
}
