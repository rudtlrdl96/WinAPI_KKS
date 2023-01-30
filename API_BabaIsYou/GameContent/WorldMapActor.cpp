#include "WorldMapActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include "ContentEnum.h"
#include "WiggleRender.h"
#include "ContentConst.h"

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

	IsWalkableValue = true;
	IsLineValue = false;
}

void WorldMapActor::InitWorldMapActorLine()
{
	PuzzleName = "";
	PuzzleInfo = "";
	PuzzleNumber = -1;

	InitWiggleRender({
	.FileName = "WorldMapLine.BMP",
	.Pos = float4::Zero,
	.Scale = {34, 34},
	.StartIndex = 0,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAPGRID),
	.BitmapInterval = 16,
	.IsUI = true });

	GetWiggleRender()->ActiveTile();
	IsWalkableValue = true;
	IsLineValue = true;
}

void WorldMapActor::SetTileIndex(const int _Key)
{
	WiggleRender* MyWiggleRenderPtr = GetWiggleRender();

	if (nullptr == MyWiggleRenderPtr)
	{
		MsgAssert("WiggleRender�� �ʱ�ȭ ���� �ʰ� ����Ϸ� �߽��ϴ�.");
		return;
	}

	MyWiggleRenderPtr->SetTileIndex(ContentConst::GetTile(_Key));
}