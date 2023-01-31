#pragma once
#include "WiggleActor.h"
#include <string>
#include <string_view>
#include "ContentEnum.h"

class WiggleRender;
class WiggleGridActor : WiggleActor
{
public:
	WiggleGridActor();
	~WiggleGridActor();

	WiggleGridActor(const WiggleGridActor& _Other) = delete;
	WiggleGridActor(WiggleGridActor&& _Other) noexcept = delete;
	WiggleGridActor& operator=(const WiggleGridActor& _Other) = delete;
	WiggleGridActor& operator=(WiggleGridActor&& _Other) noexcept = delete;

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