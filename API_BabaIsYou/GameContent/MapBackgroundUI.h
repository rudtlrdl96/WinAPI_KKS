#pragma once
#include "WiggleActor.h"

class MapBackgroundUI : public WiggleActor
{
public:
	MapBackgroundUI();
	~MapBackgroundUI();

	MapBackgroundUI(const MapBackgroundUI& _Other) = delete;
	MapBackgroundUI(MapBackgroundUI&& _Other) noexcept = delete;
	MapBackgroundUI& operator=(const MapBackgroundUI& _Other) = delete;
	MapBackgroundUI& operator=(MapBackgroundUI&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};