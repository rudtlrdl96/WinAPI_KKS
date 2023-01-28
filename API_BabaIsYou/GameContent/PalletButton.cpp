#include "PalletButton.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineRender.h>
#include "WiggleRender.h"
#include "ContentConst.h"

PalletButton::PalletButton()
{
}

PalletButton::~PalletButton()
{
}

void PalletButton::SetActorEnum(int _Enum)
{
	ActorEnumIndex = _Enum;
}

void PalletButton::SetActorImageFrame(int _Frame)
{
	ActorRenderIndex = _Frame;

	if (0 > ActorRenderIndex)
	{
		IsEmptyValue = true;
		ActorRender->Off();
		return;
	}

	IsEmptyValue = false;
	ActorRender->SetFrame(ActorRenderIndex);
	ActorRender->On();
}

void PalletButton::Start()
{
	InitRender({
	.FileName = "Pallet.BMP",
	.Pos = float4::Zero,
	.Scale = ContentConst::MAPTOOL_PALLET_SIZE,
	.StartIndex = 2,
	.AnimLength = 0,
	.Order = 10,
	.BitmapInterval = 3 }); 

	GetWiggleRender()->ActiveWiggle();

	SetColSize(ContentConst::MAPTOOL_PALLET_SIZE);

	ActorRender = CreateRender("Actor.BMP", 11);
	ActorRender->SetScale(ContentConst::ACTOR_SIZE);
	ActorRender->Off();
}

void PalletButton::Update(float _DT)
{
	WiggleActor::Update(_DT);

	if (true == IsEmptyValue)
	{
		return;
	}

	float4 CenterPos = GetPos() + ColOffset;
	float4 HalfSize = ColSize.half();
	POINT MousePos;

	if (false == GetCursorPos(&MousePos))
	{
		GetWiggleRender()->SetStartIndex(2);
		return;
	}

	ScreenToClient(GameEngineWindow::GetHWnd(), &MousePos);

	// AABB
	if (MousePos.x > CenterPos.x - HalfSize.ix() &&
		MousePos.x < CenterPos.x + HalfSize.ix() &&
		MousePos.y > CenterPos.y - HalfSize.iy() &&
		MousePos.y < CenterPos.y + HalfSize.iy())
	{
		IsOverlapValue = true;

		if (nullptr != GetWiggleRender())
		{
			GetWiggleRender()->SetStartIndex(1);
		}

		if (true == GameEngineInput::IsPress("MouseLeft"))
		{
			IsClickValue = true;
		}
		else
		{
			IsClickValue = false;
		}

		if (true == GameEngineInput::IsUp("MouseLeft"))
		{
			IsUpValue = true;
		}
		else
		{
			IsUpValue = false;
		}
	}
	else
	{
		if (nullptr != GetWiggleRender())
		{
			GetWiggleRender()->SetStartIndex(0);
		}

		IsOverlapValue = false;
		IsClickValue = false;
		IsUpValue = false;
	}
}
