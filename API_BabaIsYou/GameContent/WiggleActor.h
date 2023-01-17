#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <string_view>
#include "ContentMath.h"

class WiggleRender;
class WiggleActor : public GameEngineActor
{
public:
	WiggleActor();
	~WiggleActor();

	WiggleActor(const WiggleActor& _Other) = delete;
	WiggleActor(WiggleActor&& _Other) noexcept = delete;
	WiggleActor& operator=(const WiggleActor& _Other) = delete;
	WiggleActor& operator=(WiggleActor&& _Other) noexcept = delete;
protected:
	void Update(float _DT) override;

	void InitRender(const std::string_view& _FileName, const float4& _Pos, const float4& _Scale, int _StartIndex, int _AnimLength, int _Order = 0, int _BitmapInterval = -1);

	void SetFrame(int _Frame);
	void SetLength(int _Length);
	void SetDirInterval(size_t _DirInterval);
	void PrevAnim();
	void NextAnim();
	void SetAnimDir(const int2& _Dir);
	void RenderOn();
	void RenderOff();

	
private:
	float ProgressTime = 0.0f;
	WiggleRender* Render;
};