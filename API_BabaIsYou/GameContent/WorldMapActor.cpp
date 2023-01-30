#include "WorldMapActor.h"
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include "ContentEnum.h"

WorldMapActor::WorldMapActor()
{
}

WorldMapActor::~WorldMapActor()
{
}

void WorldMapActor::InitWorldMapActor(const std::string_view& _PuzzleName, const std::string_view& _PuzzleInfo, size_t PuzzleNumber)
{
	PuzzleName = _PuzzleName;
	PuzzleInfo = _PuzzleInfo;
	PuzzleNumber = PuzzleNumber;

	InitWiggleRender({
	.FileName = "WorldMapActorBack.BMP",
	.Pos = float4::Zero,
	.Scale = {32, 32},
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAPGRID),
	.BitmapInterval = 1,
	.IsUI = true });
}