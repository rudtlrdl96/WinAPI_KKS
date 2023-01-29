#pragma once
#include <vector>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentMath.h"

class FadeUI;
class ButtonUI;
class BlackBackUI;
class PalletActor;
class GridActor;
class MapToolGridData;
class MapToolLevel : public GameEngineLevel
{
private:
	enum class MAPTOOL_BRUSH
	{
		NONE,
		PEN,
		ERASE
	};
public:
	MapToolLevel();
	~MapToolLevel();

	MapToolLevel(const MapToolLevel& _Other) = delete;
	MapToolLevel(MapToolLevel&& _Other) noexcept = delete;
	MapToolLevel& operator=(const MapToolLevel& _Other) = delete;
	MapToolLevel& operator=(MapToolLevel&& _Other) noexcept = delete;

protected:
	void Loading() override;
	void Update(float _DT) override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;
private:
	FadeUI* MapToolFadeActor = nullptr;
	BlackBackUI* GridBackActor = nullptr;
	PalletActor* Pallet = nullptr;
	ButtonUI* PalletCloseButton = nullptr;
	ButtonUI* ObjectButton = nullptr;
	ButtonUI* PenBrushButton = nullptr;
	ButtonUI* EraseBrushButton = nullptr;	
	ButtonUI* MapSizeUpButtonY = nullptr;
	ButtonUI* MapSizeDownButtonY = nullptr;
	ButtonUI* MapSizeUpButtonX = nullptr;
	ButtonUI* MapSizeDownButtonX = nullptr;
	ButtonUI* MapSaveButton = nullptr;
	ButtonUI* MapLoadButton = nullptr;

	MAPTOOL_BRUSH BrushType = MAPTOOL_BRUSH::NONE;
	int2 MapSize = int2::Zero;
	std::vector<std::vector<MapToolGridData*>> vecMapDatas;

	MapToolGridData* SelectGrid();
	void SelectBrush(MAPTOOL_BRUSH _Brush);
	void DrawMap();
	void EraseMap();
	void DisableMap();
	void ActiveMap();
	void ResizeMap(const int2& _MapSize);

	void SaveMap();
	void LoadMap();

	void ActiveMainButton();
	void ActivePalletButton();
	void DisableMainButton();
	void DisablePalletButton();
};