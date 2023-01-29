#include "MapToolGridData.h"
#include <GameEngineCore/GameEngineRender.h>
#include"ContentConst.h"
#include "ContentDataBase.h"

MapToolGridData::MapToolGridData()
{
}

MapToolGridData::~MapToolGridData()
{
}

void MapToolGridData::Start()
{
	GridRender = CreateRender("actor.BMP", 5);
	GridRender->SetScale(ContentConst::ACTOR_SIZE);
	SetIndex(-1);
}

void MapToolGridData::Update(float _DT)
{

}

void MapToolGridData::SetIndex(int _Index)
{
	DataIndex = _Index;

	if (0 > DataIndex)
	{
		GridRender->Off();
		Off();
	}
	else
	{
		const ActorData* Data = ContentDataBase::GetInst()->GetData(_Index);
		GridRender->SetFrame(Data->RenderIndex);
		GridRender->On();
		On();
	}
}