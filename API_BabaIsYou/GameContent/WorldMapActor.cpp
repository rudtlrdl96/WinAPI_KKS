#include "WorldMapActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentEnum.h"
#include "WiggleRender.h"
#include "ContentConst.h"
#include "StringUI.h"

WorldMapActor::WorldMapActor()
{
}

WorldMapActor::~WorldMapActor()
{
}

void WorldMapActor::PuzzleClear()
{
	GetWiggleRender()->SetStartIndex(0);
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
	.StartIndex = 1,
	.AnimLength = 1,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAPGRID),
	.BitmapInterval = 2,
	.IsUI = true });

	NumberStringUI = GetLevel()->CreateActor<StringUI>();

	NumberStringUI->SetPos(GetPos() - float4{17.0f, 0.0f});
	NumberStringUI->SetFontSize({ 24, 24 });
	NumberStringUI->SetFontInterval({ 12, 10 });

	std::string WriteNumber;

	if (10 > PuzzleNumber)
	{
		WriteNumber = "0";
	}

	WriteNumber += std::to_string(PuzzleNumber);
	NumberStringUI->WriteText(WriteNumber);


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
	.Scale = {35, 35},
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
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	MyWiggleRenderPtr->SetTileIndex(ContentConst::GetTile(_Key));
}