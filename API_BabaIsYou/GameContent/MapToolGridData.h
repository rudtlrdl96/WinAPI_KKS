#pragma once
#include <GameEngineCore/GameEngineActor.h>

class GameEngineRender;
class MapToolGridData : public GameEngineActor
{
public:
	MapToolGridData();
	~MapToolGridData();

	MapToolGridData(const MapToolGridData& _Other) = delete;
	MapToolGridData(MapToolGridData&& _Other) noexcept = delete;
	MapToolGridData& operator=(const MapToolGridData& _Other) = delete;
	MapToolGridData& operator=(MapToolGridData&& _Other) noexcept = delete;

	void SetIndex(int _Index);

	inline int GetIndex() const
	{
		return DataIndex;
	}

protected:
	void Start() override;
	void Update(float _DT) override;

private:
	GameEngineRender* GridRender = nullptr;

	int DataIndex = -1;
};