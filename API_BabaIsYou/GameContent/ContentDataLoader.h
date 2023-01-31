#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include <string_view>
#include "ActorData.h"

class ContentDataLoader
{
public:
	static std::string GetOpenFilePath();
	static std::string GetSaveFilePath();
	static bool LoadMapData(const std::string_view& _Path, std::vector<std::vector<int>>& _MapLoad, std::vector<std::vector<int>>& _MapDir);
	static bool SaveMapData(const std::string_view& _Path, const std::vector<std::vector<int>>& _MapData, const std::vector<std::vector<int>>& _MapDir);
	static bool LoadActorDataBase(const std::string_view& _Path, std::map<int, ActorData>& _mapActorDatas);
protected:

private:
	ContentDataLoader();
	~ContentDataLoader();

	ContentDataLoader(const ContentDataLoader& _Other) = delete;
	ContentDataLoader(ContentDataLoader&& _Other) noexcept = delete;
	ContentDataLoader& operator=(const ContentDataLoader& _Other) = delete;
	ContentDataLoader& operator=(ContentDataLoader&& _Other) noexcept = delete;
};