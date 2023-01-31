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

void WiggleMapToolActor::SetRender(int _ActorEnum, DIR_FLAG _Dir)
{
	if (nullptr == WiggleRenderPtr)
	{
		MsgAssert("WiggleRender를 초기화 하지 않고 사용하려 했습니다.");
		return;
	}

	Index = _ActorEnum;

	if (-1 >= _ActorEnum)
	{
		Off();
		return;
	}

	On();
	const ActorData* LoadData = ContentDataBase::GetInst()->GetData(_ActorEnum);
	
	IsTileValue = false;
	WiggleRenderPtr->DisableTile();

	switch (LoadData->RenderType)
	{
	case ACTOR_RENDER_TYPE::CHARACTER:
		WiggleRenderPtr->ActiveDir();
		WiggleRenderPtr->SetDirInterval(4);
		break;
	case ACTOR_RENDER_TYPE::DYNAMIC:
		WiggleRenderPtr->ActiveDir();
		WiggleRenderPtr->SetDirInterval(1);
		break;
	case ACTOR_RENDER_TYPE::STATIC:
		WiggleRenderPtr->DisableDir();
		WiggleRenderPtr->SetDirInterval(0);
		break;
	case ACTOR_RENDER_TYPE::TILE:
		WiggleRenderPtr->DisableDir();
		IsTileValue = true;
		WiggleRenderPtr->ActiveTile();
		WiggleRenderPtr->SetDirInterval(0);
		break;
	default:
		break;
	}

	WiggleRenderPtr->SetStartIndex(LoadData->RenderIndex);

	if (DIR_FLAG::LEFT == _Dir)
	{
		WiggleRenderPtr->SetAnimDir(int2::Left);
	}
	else if (DIR_FLAG::RIGHT == _Dir)
	{
		WiggleRenderPtr->SetAnimDir(int2::Right);
	}
	else if (DIR_FLAG::UP == _Dir)
	{
		WiggleRenderPtr->SetAnimDir(int2::Up);
	}
	else if (DIR_FLAG::DOWN == _Dir)
	{
		WiggleRenderPtr->SetAnimDir(int2::Down);
	}

	DirNumber = _Dir;
}

void WiggleMapToolActor::SetRender(const std::string_view& _Name, DIR_FLAG _Dir)
{
	SetRender(ContentDataBase::GetInst()->GetActorEnum(_Name), _Dir);
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
	if (false == IsTileValue)
	{
		return;
	}

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
	.Order = 5,
	.BitmapInterval = 24 });

	WiggleRenderPtr = GetWiggleRender();
}
