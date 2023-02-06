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
    return "";
}

bool ContentDataLoader::LoadMapData(const std::string_view& _Path, std::vector<std::vector<int>>& _MapLoad, std::vector<std::vector<int>>& _MapDir)
{
    if ("" == _Path)
    {
        return false;
    }

    if (0 < _MapLoad.size())
    {
        for (size_t y = 0; y < _MapLoad.size(); y++)
        {
            _MapLoad[y].clear();
        }

        _MapLoad.clear();
    }

    if (0 < _MapDir.size())
    {
        for (size_t y = 0; y < _MapDir.size(); y++)
        {
            _MapDir[y].clear();
        }

        _MapDir.clear();
    }

    std::ifstream OpenMapStream;
    OpenMapStream.open(_Path.data(), std::ios_base::in);

    if (false == OpenMapStream.is_open())
    {
        MsgAssert("맵 데이터를 불러오는데 실패했습니다.");
        return false;
    }

    std::string DataLineString;
    std::string DataPosString;

    int DataIndexY = 0;
    while (std::getline(OpenMapStream, DataLineString))
    {
        std::stringstream LineStream(DataLineString);
        _MapLoad.push_back(std::vector<int>());

        while (std::getline(LineStream, DataPosString, ','))
        {
            _MapLoad[DataIndexY].push_back(std::stoi(DataPosString));
        }

        ++DataIndexY;
    }

    OpenMapStream.close();

    std::ifstream OpenDirStream;

    std::string DirPath = _Path.data();
    DirPath += "_Dir";
    OpenDirStream.open(DirPath, std::ios_base::in);

    if (false == OpenDirStream.is_open())
    {
        MsgAssert("맵 데이터를 불러오는데 실패했습니다.");
        return false;
    }

    std::string DirLineString;
    std::string DirPosString;

    int DirIndexY = 0;
    while (std::getline(OpenDirStream, DirLineString))
    {
        std::stringstream LineStream(DirLineString);
        _MapDir.push_back(std::vector<int>());

        while (std::getline(LineStream, DirPosString, ','))
        {
            _MapDir[DirIndexY].push_back(std::stoi(DirPosString));
        }

        ++DirIndexY;
    }

    OpenDirStream.close();


    return true;
}

bool ContentDataLoader::SaveMapData(const std::string_view& _Path, const std::vector<std::vector<int>>& _MapData, const std::vector<std::vector<int>>& _MapDir)
{
    if ("" == _Path)
    {
        return false;
    }

    std::ofstream MapSaveStream;
    MapSaveStream.open(_Path.data());

    if (false == MapSaveStream.is_open())
    {
        MsgAssert("맵 데이터를 저장하는데 실패했습니다.");
        return false;
    }

    for (size_t y = 0; y < _MapData.size(); y++)
    {
        for (size_t x = 0; x < _MapData[y].size(); x++)
        {
            MapSaveStream << _MapData[y][x];

            if (_MapData[y].size() - 1 != x)
            {
                MapSaveStream << ",";
            }
        }

        MapSaveStream << "\n";
    }

    MapSaveStream.close();

    std::ofstream DirSaveStream;

    std::string DirPath = _Path.data();
    DirPath += "_Dir";

    DirSaveStream.open(DirPath);

    if (false == DirSaveStream.is_open())
    {
        MsgAssert("맵 데이터를 저장하는데 실패했습니다.");
        return false;
    }

    for (size_t y = 0; y < _MapDir.size(); y++)
    {
        for (size_t x = 0; x < _MapDir[y].size(); x++)
        {
            DirSaveStream << _MapDir[y][x];

            if (_MapDir[y].size() - 1 != x)
            {
                DirSaveStream << ",";
            }
        }

        DirSaveStream << "\n";
    }

    DirSaveStream.close();

    return true;
}

bool ContentDataLoader::LoadActorDataBase(const std::string_view& _Path, std::map<int, ActorData>& _mapActorDatas)
{
    static int LoadIndex = 0;

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
        ActorData LoadData(LoadIndex++);

        std::stringstream LineStream(LineData);

        std::getline(LineStream, ReadData, ','); // Name
        LoadData.ActorName = GameEngineString::ToUpper(ReadData);
        
        std::getline(LineStream, ReadData, ','); // Index
        LoadData.RenderIndex = std::stoi(ReadData);

        std::getline(LineStream, ReadData, ','); // Color
        std::string ColorName = GameEngineString::ToUpper(ReadData);

        if ("WHITE" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::WHITE;
        }
        else if ("BURGUNDY" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::BURGUNDY;
        }
        else if ("GRAY" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::GRAY;
        }
        else if ("JAVA" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::JAVA;
        }
        else if ("ANZAC" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::ANZAC;
        }
        else if ("SUSHI" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::SUSHI;
        }
        else if ("CLAY" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::CLAY;
        }
        else if ("MOCHA" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::MOCHA;
        }
        else if ("FLAX" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::FLAX;
        }
        else if ("ROYALBLUE" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::ROYALBLUE;
        }
        else if ("BLACK" == ColorName)
        {
            LoadData.Color = PARTICLE_COLOR::BLACK;
        }

        std::getline(LineStream, ReadData, ','); // ActorType
        ReadData = GameEngineString::ToUpper(ReadData);

        if ("ACTOR" == ReadData)
        {
            LoadData.ActorType = ACTOR_TYPE::ACTOR;

            ActorData ActorText(LoadData.ActorEnum + 10000);
            ActorText.ActorName = LoadData.ActorName + "_TEXT";
            ActorText.RenderIndex = LoadData.RenderIndex - 2;
            ActorText.RenderOrder = 1000;
            ActorText.Color = LoadData.Color;
            ActorText.ActorType = ACTOR_TYPE::SUBJECT_TEXT;
            ActorText.RenderType = ACTOR_RENDER_TYPE::STATIC;
            ActorText.ArrowEnum = LoadData.ActorEnum;
            ActorText.ArrowDefine = ACTOR_DEFINE::NONE;

            _mapActorDatas.insert({ ActorText.ActorEnum, ActorText });
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

        std::getline(LineStream, ReadData, ','); // RenderType
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
        else if ("STATIC" == ReadData)
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::STATIC;
        }
        else if ("DYNAMIC" == ReadData)
        {
            LoadData.RenderType = ACTOR_RENDER_TYPE::DYNAMIC;
        }
        else
        {
            MsgAssert("액터 데이터 로드 실패 잘못된 랜더 타입입니다");
        }

        std::getline(LineStream, ReadData, ','); // RenderOrder
        ReadData = GameEngineString::ToUpper(ReadData);

        LoadData.RenderOrder = std::stoi(ReadData);

        if ("YOU" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::YOU;
        }
        else if ("WIN" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::WIN;
        }
        else if ("STOP" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::STOP;
        }
        else if ("PUSH" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::PUSH;
        }
        else if ("SINK" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::SINK;
        }
        else if ("DEFEAT" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::DEFEAT;
        }
        else if ("HOT" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::HOT;
        }
        else if ("MELT" == LoadData.ActorName)
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::MELT;
        }
        else
        {
            LoadData.ArrowDefine = ACTOR_DEFINE::NONE;
        }

        _mapActorDatas.insert({LoadData.ActorEnum, LoadData});
    }


    return false;
}
