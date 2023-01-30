#pragma once
#include <string>
#include <string_view>
#include "ContentMath.h"
#include "WiggleActor.h"

class WorldMapActor : public WiggleActor
{
public:
	WorldMapActor();
	~WorldMapActor();

	WorldMapActor(const WorldMapActor& _Other) = delete;
	WorldMapActor(WorldMapActor&& _Other) noexcept = delete;
	WorldMapActor& operator=(const WorldMapActor& _Other) = delete;
	WorldMapActor& operator=(WorldMapActor&& _Other) noexcept = delete;

	inline void SetGridPos(const int2& _Pos)
	{
		GridPos = _Pos;
	}

	inline std::string_view GetPuzzleName() const
	{
		return PuzzleName;
	}

	void InitWorldMapActor(const std::string_view& _PuzzleName, const std::string_view& _PuzzleInfo, size_t PuzzleNumber);

protected:

private:
	int2 GridPos = int2::Zero;

	std::string PuzzleName = "";
	std::string PuzzleInfo = "";
	size_t PuzzleNumber = 0;

};