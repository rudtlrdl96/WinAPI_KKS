#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <string_view>

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

	static size_t GetWiggleIndex()
	{
		return WiggleIndex;
	}

protected:
	void InitRender(const std::string_view& _FileName, const float4& _Pos, const float4& _Scale, int _StartIndex, int _AnimLength, int _Order = 0, int _Interval = -1);

private:
	static size_t WiggleIndex;
	static void Wiggle();

	WiggleRender* Render;

	friend class WiggleManager;
};