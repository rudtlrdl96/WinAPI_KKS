#include "WorldMapSelect.h"
#include "ContentEnum.h"

WorldMapSelect::WorldMapSelect()
{
}

WorldMapSelect::~WorldMapSelect()
{
}

void WorldMapSelect::Start()
{
	InitWiggleRender({
	.FileName = "WorldMapSelect.BMP",
	.Pos = float4::Zero,
	.Scale = {45, 45},
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAPSELECT),
	.BitmapInterval = 1,
	.IsUI = true });
}
