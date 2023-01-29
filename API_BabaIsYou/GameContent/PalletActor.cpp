#include "PalletActor.h"
#include <GameEngineCore/GameEngineRender.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "WiggleRender.h"
#include "ContentDataBase.h"
#include "ContentConst.h"
#include "ContentEnum.h"
#include "PalletButton.h"
#include "PalletSelect.h"
#include "ButtonUI.h"

PalletActor::PalletActor()
{
}

PalletActor::~PalletActor()
{
}

void PalletActor::Start()
{
	vecLoadActors.reserve(100);

	for (size_t i = 0; i < vecLoadActors.capacity(); i++)
	{
		PalletButton* CreateButton = GetLevel()->CreateActor<PalletButton>();
		vecLoadActors.push_back(CreateButton);
		vecLoadActors[i]->SetPos(
			{ i % 10 * ContentConst::MAPTOOL_PALLET_SIZE.x,
			static_cast<int>(i / 10) * ContentConst::MAPTOOL_PALLET_SIZE.y });
		vecLoadActors[i]->SetMove(ContentConst::MAPTOOL_PALLET_SIZE);
	}

	const std::map<int, ActorData>& LoadDatas = ContentDataBase::GetInst()->GetAllData();

	int Index = 0;

	for (const std::pair<int, ActorData>& Data : LoadDatas)
	{
		if (Index >= vecLoadActors.size())
		{
			MsgAssert("최대 팔레트 수를 초과했습니다");
			return;
		}

		vecLoadActors[Index]->SetActorEnum(Data.second.ActorEnum);
		vecLoadActors[Index]->SetActorImageFrame(Data.second.RenderIndex);
		++Index;
	}

	PalletSelectActor = GetLevel()->CreateActor<PalletSelect>();
	SelectPen(0);
}

void PalletActor::Update(float _DT)
{
	for (size_t i = 0; i < vecLoadActors.size(); i++)
	{
		if (true == vecLoadActors[i]->IsUp())
		{
			SelectPen(i);
		}
	}
}

void PalletActor::SelectPen(size_t _Index)
{
	if (0 > _Index || vecLoadActors.size() <= _Index)
	{
		MsgAssert("벡터 인덱스 범위를 벗어나 접근했습니다.");
		return;
	}

	PenEnum = vecLoadActors[_Index]->GetActorEnum();
	PalletSelectActor->SetPos(vecLoadActors[_Index]->GetPos());
}

void PalletActor::ActivePallet()
{
	for (size_t i = 0; i < vecLoadActors.size(); i++)
	{
		vecLoadActors[i]->On();
	}

	PalletSelectActor->On();
	On();
}

void PalletActor::DisablePallet()
{
	Off();
	PalletSelectActor->Off();

	for (size_t i = 0; i < vecLoadActors.size(); i++)
	{
		vecLoadActors[i]->Off();
	}
}