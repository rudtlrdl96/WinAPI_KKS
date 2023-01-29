#include "PalletSelect.h"
#include "ContentConst.h"

PalletSelect::PalletSelect()
{
}

PalletSelect::~PalletSelect()
{
}

void PalletSelect::Start()
{
	InitRender({
	.FileName = "SelectPallet.BMP",
	.Pos = float4::Zero,
	.Scale = ContentConst::MAPTOOL_PALLET_SIZE,
	.StartIndex = 0,
	.AnimLength = 0,
	.Order = 15,
	.BitmapInterval = 1,
	.IsUI = true});
}