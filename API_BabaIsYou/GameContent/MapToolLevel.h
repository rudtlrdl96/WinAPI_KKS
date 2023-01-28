#pragma once
#include <vector>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentMath.h"

class FadeUI;
class ButtonUI;
class BlackBackUI;
class PalletActor;
class GridActor;
class MapGridData;
class MapToolLevel : public GameEngineLevel
{
public:
	MapToolLevel();
	~MapToolLevel();

	MapToolLevel(const MapToolLevel& _Other) = delete;
	MapToolLevel(MapToolLevel&& _Other) noexcept = delete;
	MapToolLevel& operator=(const MapToolLevel& _Other) = delete;
	MapToolLevel& operator=(MapToolLevel&& _Other) noexcept = delete;

	void SetSize(int2 _Size);

protected:
	void Loading() override;
	void Update(float _DT) override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;
private:
	FadeUI* MapToolFadeActor = nullptr;
	BlackBackUI* GridBackActor = nullptr;
	PalletActor* Pallet = nullptr;
	ButtonUI* ObjectButton = nullptr;

	int2 MapSize = int2::Zero;
	std::vector<std::vector<MapGridData*>> vecMapDatas;

	void DisableMap();
	void ActiveMap();
	void ResizeMap(const int2& _MapSize);
};