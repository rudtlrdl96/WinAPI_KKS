#pragma once
#include <GameEngineCore/GameEngineActor.h>

class GridActor : public GameEngineActor
{
public:
	GridActor();
	~GridActor();

	GridActor(const GridActor& _Other) = delete;
	GridActor(GridActor&& _Other) noexcept = delete;
	GridActor& operator=(const GridActor& _Other) = delete;
	GridActor& operator=(GridActor&& _Other) noexcept = delete;

	//const float4 GetIndex() const
	//{
	//	return float4(static_cast x, y);
	//}

	void SetPos(size_t _x, size_t _y)
	{
		x = _x;
		y = _y;
	}

protected:

private:
	size_t x;
	size_t y;
};