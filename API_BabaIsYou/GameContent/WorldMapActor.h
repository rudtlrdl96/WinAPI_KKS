#pragma once
#include <string>
#include <string_view>
#include "ContentMath.h"
#include "WiggleActor.h"

class StringUI;
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

	inline std::string_view GetPuzzleInfo() const
	{
		return PuzzleInfo;
	}

	inline bool IsWalkable() const
	{
		return IsWalkableValue;
	}

	inline bool IsLine() const
	{
		return IsLineValue;
	}

	void PuzzleClear();
	void InitWorldMapActor(const std::string_view& _PuzzleName, const std::string_view& _PuzzleInfo, size_t PuzzleNumber);
	void InitWorldMapActorLine();
	void SetTileIndex(const int _Key);

protected:

private:
	int2 GridPos = int2::Zero;

	std::string PuzzleName = "";
	std::string PuzzleInfo = "";
	StringUI* NumberStringUI = nullptr;

	size_t PuzzleNumber = 0;

	bool IsWalkableValue = false;
	bool IsLineValue = false;
};