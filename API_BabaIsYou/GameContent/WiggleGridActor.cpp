#include "WiggleGridActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentConst.h"
#include "WiggleRender.h"
#include "ContentDataBase.h"

WiggleGridActor::WiggleGridActor()
{
}

WiggleGridActor::~WiggleGridActor()
{
}

void WiggleGridActor::SetRender(int _ActorEnum)
{
	ContentDataBase::GetInst()->GetData(_ActorEnum);
}

void WiggleGridActor::SetRender(const std::string_view& _Name)
{
	ContentDataBase::GetInst()->GetData(_Name);
}

void WiggleGridActor::SetDir(int2 _Dir)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	WiggleRenderPtr->SetAnimDir(_Dir);
}

void WiggleGridActor::SetTile(int _Key)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	WiggleRenderPtr->SetTileIndex(ContentConst::GetTile(_Key));
}

void WiggleGridActor::Start()
{
	InitWiggleRender({
	.FileName = "actor.BMP",
	.Pos = float4::Zero,
	.Scale = ContentConst::GRID_BITMAP_SIZE,
	.StartIndex = 0,
	.AnimLength = 4,
	.Order = static_cast<int>(RENDER_ORDER::WORLDMAPGRID),
	.BitmapInterval = 24 });

	WiggleRenderPtr = GetWiggleRender();
}
