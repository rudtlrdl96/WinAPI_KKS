#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <vector>
#include "ContentMath.h"

class FadeUI;
class WorldMapActor;
class WorldMapSelect;
class StringUI;
class SoundSystem;
class WorldMapLevel : public GameEngineLevel
{
public:
	WorldMapLevel();
	~WorldMapLevel();

	WorldMapLevel(const WorldMapLevel& _Other) = delete;
	WorldMapLevel(WorldMapLevel&& _Other) noexcept = delete;
	WorldMapLevel& operator=(const WorldMapLevel& _Other) = delete;
	WorldMapLevel& operator=(WorldMapLevel&& _Other) noexcept = delete;

protected:

	void Loading() override;
	void Update(float _DT) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}

private:
	FadeUI* WorldMapFadeActor = nullptr;
	std::vector<std::vector<WorldMapActor*>> vecWorldMapDatas;
	WorldMapSelect* WorldMapSelectActor = nullptr;
	StringUI* InfoStringUI = nullptr;
	SoundSystem* SoundSystemPtr = nullptr;

	int2 MapSize = int2::Zero;
	int2 SelectPuzzlePos = int2::Zero;

	void MoveWorldMap(const int2& _Move);
	void LineActorInit(const int2& _Pos);
	bool MapOverCheck(const int2& _Pos) const;
};