#pragma once
#include "SubjectActor.h"

enum TILE_STATE
{
	NONE	= 0,
	LEFT	= 1 << 0,
	RIGHT	= 1 << 1,
	UP		= 1 << 2,
	DOWN	= 1 << 3,
};

class TileActor : public SubjectActor
{
public:
	TileActor();
	~TileActor();

	TileActor(const TileActor& _Other) = delete;
	TileActor(TileActor&& _Other) noexcept = delete;
	TileActor& operator=(const TileActor& _Other) = delete;
	TileActor& operator=(TileActor&& _Other) noexcept = delete;

protected:

private:

	void SetTileRender();

};