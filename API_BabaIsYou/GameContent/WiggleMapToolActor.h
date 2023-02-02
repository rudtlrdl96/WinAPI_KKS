#pragma once
#include "WiggleActor.h"
#include <string>
#include <string_view>
#include "ContentEnum.h"
#include "ContentMath.h"

class WiggleRender;
class WiggleMapToolActor : public WiggleActor
{
public:
	WiggleMapToolActor();
	~WiggleMapToolActor();

	WiggleMapToolActor(const WiggleMapToolActor& _Other) = delete;
	WiggleMapToolActor(WiggleMapToolActor&& _Other) noexcept = delete;
	WiggleMapToolActor& operator=(const WiggleMapToolActor& _Other) = delete;
	WiggleMapToolActor& operator=(WiggleMapToolActor&& _Other) noexcept = delete;

	void SetRender(int _ActorEnum, DIR_FLAG _Dir);
	void SetRender(const std::string_view& _Name, DIR_FLAG _Dir);

	void SetDir(int2 _Dir);
	void SetTile(int _Key);

	inline int GetIndex() const
	{
		return Index;
	}

	inline bool IsTile() const
	{
		return IsTileValue;
	}

	inline void SetGridPos(const int2& _Pos)
	{
		GridPos = _Pos;
	}

	inline int2 GetGridPos() const
	{
		return GridPos;
	}

	inline DIR_FLAG GetDir() const
	{
		return DirNumber;
	}

	inline WiggleRender* GetRender() const
	{
		return WiggleRenderPtr;
	}

protected:
	void Start() override;

private:
	WiggleRender* WiggleRenderPtr = nullptr;
	int Index = -1;
	bool IsTileValue = false;
	int2 GridPos = int2::Zero;
	DIR_FLAG DirNumber = DIR_FLAG::RIGHT;
};