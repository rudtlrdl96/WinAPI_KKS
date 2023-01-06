#pragma once
#include <GameEngineCore/GameEngineLevel.h>

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
	void Update() override;

private:

};