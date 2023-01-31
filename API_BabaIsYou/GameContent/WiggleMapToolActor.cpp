#include "WiggleMapToolActor.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "ContentConst.h"
#include "WiggleRender.h"
#include "ContentDataBase.h"
#include "ActorData.h"

WiggleMapToolActor::WiggleMapToolActor()
{
}

WiggleMapToolActor::~WiggleMapToolActor()
{
}

void WiggleMapToolActor::SetRender(int _ActorEnum)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	const ActorData* LoadData = ContentDataBase::GetInst()->GetData(_ActorEnum);

	WiggleRenderPtr->SetStartIndex(LoadData->RenderIndex);
	
}

void WiggleMapToolActor::SetRender(const std::string_view& _Name)
{
	SetRender(ContentDataBase::GetInst()->GetActorEnum(_Name));
}

void WiggleMapToolActor::SetDir(int2 _Dir)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	WiggleRenderPtr->SetAnimDir(_Dir);
}

void WiggleMapToolActor::SetTile(int _Key)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	WiggleRenderPtr->SetTileIndex(ContentConst::GetTile(_Key));
}

void WiggleMapToolActor::Start()
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
