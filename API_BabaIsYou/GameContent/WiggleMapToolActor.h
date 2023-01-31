#pragma once
#include "WiggleActor.h"
#include <string>
#include <string_view>
#include "ContentEnum.h"

class WiggleRender;
class WiggleMapToolActor: WiggleActor
{
public:
	WiggleMapToolActor();
	~WiggleMapToolActor();

	WiggleMapToolActor(const WiggleMapToolActor& _Other) = delete;
	WiggleMapToolActor(WiggleMapToolActor&& _Other) noexcept = delete;
	WiggleMapToolActor& operator=(const WiggleMapToolActor& _Other) = delete;
	WiggleMapToolActor& operator=(WiggleMapToolActor&& _Other) noexcept = delete;

	void SetRender(int _ActorEnum);
	void SetRender(const std::string_view& _Name);

	void SetDir(int2 _Dir);
	void SetTile(int _Key);

protected:
	void Start() override;

private:
	WiggleRender* WiggleRenderPtr = nullptr;
	bool IsTile = false;
};