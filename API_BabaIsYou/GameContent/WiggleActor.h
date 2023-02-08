#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <string_view>
#include "ContentMath.h"


class WiggleAnimationParameter
{
public:
	const std::string_view& FileName = "";
	const float4& Pos = float4::Zero;
	const float4& Scale = float4::Zero;
	int StartIndex = 0;
	int AnimLength = 0;
	int Order = 0;
	int BitmapInterval = -1;
	bool IsUI = false;
};

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
	WiggleRender* InitWiggleRender(const WiggleAnimationParameter& _Parameter);
	WiggleRender* GetWiggleRender() const;
private:
	float ProgressTime = 0.0f;
	WiggleRender* Render;
};