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
    MsgAssert("��� �ҷ����� ����");
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
    MsgAssert("��� �ҷ����� ����");
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
        MsgAssert("�� �����͸� �ҷ����µ� �����߽��ϴ�.");
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
        MsgAssert("�� �����͸� �����ϴµ� �����߽��ϴ�.");
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
    static int LoadIndex = 0;

    std::ifstream OpenStream;
    OpenStream.open(_Path.data(), std::ios_base::in);

    if (false == OpenStream.is_open())
    {
        MsgAssert("���� �����͸� �ҷ����µ� �����߽��ϴ�.");
        return false;
    }

    std::string LineData;
    std::string ReadData;

    std::getline(OpenStream, LineData); // ù ���� ���õȴ�

    while (std::getline(OpenStream, LineData))
    {
        ActorData LoadData(LoadIndex++);

        std::stringstream LineStream(LineData);

        std::getline(LineStream, ReadData, ','); // Name
        LoadData.ActorName = GameEngineString::ToUpper(ReadData);
        
        std::getline(LineStream, ReadData, ','); // Index
        LoadData.RenderIndex = std::stoi(ReadData);

        std::getline(LineStream, ReadData, ','); // Color
        LoadData.Color = GameEngineString::ToUpper(ReadData);

        std::getline(LineStream, ReadData, ','); // ActorType
        ReadData = GameEngineString::ToUpper(ReadData);

        if ("ACTOR" == ReadData)
        {
            LoadData.ActorType = ACTOR_TYPE::ACTOR;

            ActorData ActorText(LoadData.ActorEnum + 10000);
            ActorText.ActorName = LoadData.ActorName + "_TEXT";
            ActorText.RenderIndex = LoadData.RenderIndex - 2;
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
            MsgAssert("���� ������ �ε� ���� �߸��� ���� Ÿ���Դϴ�");
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
            MsgAssert("���� ������ �ε� ���� �߸��� ���� Ÿ���Դϴ�");
        }

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
