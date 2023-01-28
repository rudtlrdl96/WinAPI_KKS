#pragma once
#include <GameEngineCore/GameEngineActor.h>

class MapGridData : public GameEngineActor
{
public:
	MapGridData();
	~MapGridData();

	MapGridData(const MapGridData& _Other) = delete;
	MapGridData(MapGridData&& _Other) noexcept = delete;
	MapGridData& operator=(const MapGridData& _Other) = delete;
	MapGridData& operator=(MapGridData&& _Other) noexcept = delete;

protected:

private:

};