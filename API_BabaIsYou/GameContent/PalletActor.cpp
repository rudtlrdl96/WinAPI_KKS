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

	LeftDirButton = GetLevel()->CreateActor<ButtonUI>();
	LeftDirButton->SetImage("DirLeftButton.BMP", { 80, 80 });
	LeftDirButton->SetPos({920, 200});

	RightDirButton = GetLevel()->CreateActor<ButtonUI>();
	RightDirButton->SetImage("DirRightButton.BMP", { 80, 80 });
	RightDirButton->SetPos({ 1080, 200 });

	UpDirButton = GetLevel()->CreateActor<ButtonUI>();
	UpDirButton->SetImage("DirUpButton.BMP", { 80, 80 });
	UpDirButton->SetPos({ 1000, 120 });

	DownDirButton = GetLevel()->CreateActor<ButtonUI>();
	DownDirButton->SetImage("DirDownButton.BMP", { 80, 80 });
	DownDirButton->SetPos({ 1000, 280 });

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

	if (true == LeftDirButton->IsUp())
	{
		SetDir(DIR_FLAG::LEFT);
		LeftDirButton->SetSelect(true);
	}
	else if (true == RightDirButton->IsUp())
	{
		SetDir(DIR_FLAG::RIGHT);
		RightDirButton->SetSelect(true);
	}
	else if (true == UpDirButton->IsUp())
	{
		SetDir(DIR_FLAG::UP);
		UpDirButton->SetSelect(true);
	}
	else if (true == DownDirButton->IsUp())
	{
		SetDir(DIR_FLAG::DOWN);
		DownDirButton->SetSelect(true);
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

void PalletActor::SetDir(DIR_FLAG _Dir)
{
	UpDirButton->SetSelect(false);
	DownDirButton->SetSelect(false);
	LeftDirButton->SetSelect(false);
	RightDirButton->SetSelect(false);
	PalletActorDir = _Dir;
}

void PalletActor::ActivePallet()
{
	for (size_t i = 0; i < vecLoadActors.size(); i++)
	{
		vecLoadActors[i]->On();
	}

	LeftDirButton->ButtonOn();
	RightDirButton->ButtonOn();
	UpDirButton->ButtonOn();
	DownDirButton->ButtonOn();

	PalletSelectActor->On();
	On();
}

void PalletActor::DisablePallet()
{
	Off();
	PalletSelectActor->Off();

	LeftDirButton->ButtonOff();
	RightDirButton->ButtonOff();
	UpDirButton->ButtonOff();
	DownDirButton->ButtonOff();

	for (size_t i = 0; i < vecLoadActors.size(); i++)
	{
		vecLoadActors[i]->Off();
	}
}