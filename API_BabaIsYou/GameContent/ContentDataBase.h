#pragma once
#include <map>
#include "ActorData.h"

class ContentDataBase
{
public:
	static ContentDataBase* GetInst()
	{
		static ContentDataBase Instance;
		return &Instance;
	}

	void InitDataBase();
	int  GetActorDataIndex(const std::string_view& _Name);
	const ActorData* GetData(int _Index);
	const ActorData* GetData(const std::string_view& _Name);
	const std::map<int, ActorData>& GetAllData();

protected:

private:
	ContentDataBase();
	~ContentDataBase();

	ContentDataBase(const ContentDataBase& _Other) = delete;
	ContentDataBase(ContentDataBase&& _Other) noexcept = delete;
	ContentDataBase& operator=(const ContentDataBase& _Other) = delete;
	ContentDataBase& operator=(ContentDataBase&& _Other) noexcept = delete;

	std::map<int, ActorData> mapActorDB;
	std::map<std::string, int> mapActorIndex;
};