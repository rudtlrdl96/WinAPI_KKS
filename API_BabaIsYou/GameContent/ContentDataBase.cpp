#include "ContentDataBase.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include "ContentDataLoader.h"

ContentDataBase::ContentDataBase()
{
}

ContentDataBase::~ContentDataBase()
{
}


void ContentDataBase::InitDataBase()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("Data");
	Dir.Move("Data");
	Dir.Move("Actor");
	 
	ContentDataLoader::LoadActorDataBase(Dir.GetPlusFileName("ActorDataBase.csv").GetPathToString(), mapActorDB);

	for (const std::pair<int, ActorData>& Data : mapActorDB)
	{
		mapActorIndex.insert({ Data.second.ActorName, Data.second.ActorEnum});
	}
}

int ContentDataBase::GetActorEnum(const std::string_view& _Name)
{
	return mapActorIndex[_Name.data()];
}


const ActorData* ContentDataBase::GetData(int _Index)
{
	std::map<int, ActorData>::iterator FindIter = mapActorDB.find(_Index);

	if (FindIter == mapActorDB.end())
	{
		MsgAssert("잘못된 액터 데이터 베이스 인덱스입니다.");
		return nullptr;
	}
	else
	{
		return &FindIter->second;
	}
}

const ActorData* ContentDataBase::GetData(const std::string_view& _Name)
{
	return GetData(mapActorIndex[_Name.data()]);
}

const std::map<int, ActorData>& ContentDataBase::GetAllData()
{
	return mapActorDB;
}
