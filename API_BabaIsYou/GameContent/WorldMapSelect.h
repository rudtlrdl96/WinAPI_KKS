#pragma once

#include "WiggleActor.h"

class WorldMapSelect : public WiggleActor
{
public:
	WorldMapSelect();
	~WorldMapSelect();

	WorldMapSelect(const WorldMapSelect& _Other) = delete;
	WorldMapSelect(WorldMapSelect&& _Other) noexcept = delete;
	WorldMapSelect& operator=(const WorldMapSelect& _Other) = delete;
	WorldMapSelect& operator=(WorldMapSelect&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};