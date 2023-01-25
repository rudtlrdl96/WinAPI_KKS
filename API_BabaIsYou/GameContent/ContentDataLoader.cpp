#include "ContentDataLoader.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineString.h>
#include <GameEnginePlatform/GameEngineWindow.h>

ContentDataLoader::ContentDataLoader()
{
}

ContentDataLoader::~ContentDataLoader()
{
}

std::string ContentDataLoader::GetOpenFilePath()
{
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    char PrevPath[260];
    GetCurrentDirectory(260, PrevPath);

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GameEngineWindow::GetHWnd();
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetOpenFileName(&ofn) == TRUE)
    {
        SetCurrentDirectory(PrevPath);
        return ofn.lpstrFile;
    }

    SetCurrentDirectory(PrevPath);
    MsgAssert("경로 불러오기 실패");
    return "";
}

std::string ContentDataLoader::GetSaveFilePath()
{
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    char PrevPath[260];
    GetCurrentDirectory(260, PrevPath);

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GameEngineWindow::GetHWnd();
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetSaveFileName(&ofn) == TRUE)
    {
        SetCurrentDirectory(PrevPath);
        return ofn.lpstrFile;
    }

    SetCurrentDirectory(PrevPath);
    MsgAssert("경로 불러오기 실패");
    return "";
}

bool ContentDataLoader::LoadMapData(const std::string_view& _Path, std::vector<std::vector<int>>& _MapLoad)
{
    if (0 < _MapLoad.size())
    {
        for (size_t y = 0; y < _MapLoad.size(); y++)
        {
            _MapLoad[y].clear();
        }

        _MapLoad.clear();
    }

    std::ifstream OpenStream;
    OpenStream.open(_Path.data(), std::ios_base::in);


    if (false == OpenStream.is_open())
    {
        MsgAssert("맵 데이터를 불러오는데 실패했습니다.");
        return false;
    }

    std::string LineString;
    std::string PosString;

    int IndexY = 0;
    while (std::getline(OpenStream, LineString))
    {
        std::stringstream LineStream(LineString);
        _MapLoad.push_back(std::vector<int>());

        while (std::getline(LineStream, PosString, ','))
        {
            _MapLoad[IndexY].push_back(std::stoi(PosString));
        }

        ++IndexY;
    }

    OpenStream.close();
    return true;
}

bool ContentDataLoader::SaveMapData(const std::string_view& _Path, const std::vector<std::vector<int>>& _MapData)
{
    std::ofstream SaveStream;
    SaveStream.open(_Path.data());

    if (false == SaveStream.is_open())
    {
        MsgAssert("맵 데이터를 저장하는데 실패했습니다.");
        return false;
    }

    for (size_t y = 0; y < _MapData.size(); y++)
    {
        for (size_t x = 0; x < _MapData[y].size(); x++)
        {
            SaveStream << _MapData[y][x];

            if (_MapData[y].size() - 1 != x)
            {
                SaveStream << ",";
            }
        }

        SaveStream << "\n";
    }

    SaveStream.close();
    return true;
}

bool ContentDataLoader::LoadActorDataBase(const std::string_view& _Path, std::map<int, ActorData>& _mapActorDatas)
{
    std::ifstream OpenStream;
    OpenStream.open(_Path.data(), std::ios_base::in);

    if (false == OpenStream.is_open())
    {
        MsgAssert("액터 데이터를 불러오는데 실패했습니다.");
        return false;
    }

    std::string LineData;
    std::string ReadData;

    std::getline(OpenStream, LineData); // 첫 줄은 무시된다

    while (std::getline(OpenStream, LineData))
    {
        ActorData LoadData(static_cast<int>(_mapActorDatas.size()));

        std::stringstream LineStream(LineData);

        std::getline(LineStream, ReadData, ','); // Name
        LoadData.ActorName = ReadData;
        
        std::getline(LineStream, ReadData, ','); // Index
        LoadData.RenderIndex = std::stoi(ReadData);

        std::getline(LineStream, ReadData, ','); // Color
        LoadData.Color = ReadData;

        std::getline(LineStream, ReadData, ','); // ActorType
        ReadData = GameEngineString::ToUpper(ReadData);

        if ("ACTOR" == ReadData)
        {
            LoadData.ActorType = ACTOR_TYPE::ACTOR;
        }
        else if ("VERBTEXT" == ReadData)
        {
            LoadData.ActorType = ACTOR_TYPE::VERB_TEXT;
        }
        else if ("DEFINETEXT")
        {
            LoadData.ActorType = ACTOR_TYPE::DEFINE_TEXT;
        }
        else
        {
            MsgAssert("액터 데이터 로드 실패 잘못된 액터 타입입니다");
        }

        std::getline(LineStream, ReadData, ','); // ActorType
        ReadData = GameEngineString::ToUpper(ReadData);

        if ("CHARACTER" == ReadData)
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::CHARACTER;



        }
        else if ("BELT" == ReadData)
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::BELT;
        }
        else if ("TILE" == ReadData)
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::TILE;
        }
        else if ("STATIC")
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::STATIC;
        }
        else if ("DYNAMIC")
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::DYNAMIC;
        }
        else
        {
            MsgAssert("액터 데이터 로드 실패 잘못된 랜더 타입입니다");
        }

        _mapActorDatas.insert({LoadData.ActorEnum, LoadData});
    }


    return false;
}
