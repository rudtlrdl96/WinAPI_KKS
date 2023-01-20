#pragma once
#include <GameEngineCore/GameEngineCore.h>

class ContentFunc
{
public:
	static void ChangeLogoLevel()
	{
		GameEngineCore::GetInst()->ChangeLevel("Logo");
	}

	static void ChangeTitleLevel()
	{
		GameEngineCore::GetInst()->ChangeLevel("Title");
	}

	static void ChangeWorldmapLevel()
	{
		GameEngineCore::GetInst()->ChangeLevel("Worldmap");
	}

	static void ChangePuzzleLevel()
	{
		GameEngineCore::GetInst()->ChangeLevel("Puzzle");
	}

	static void ChangeEndingLevel()
	{
		GameEngineCore::GetInst()->ChangeLevel("Ending");
	}

protected:

private:
	ContentFunc();
	~ContentFunc();

	ContentFunc(const ContentFunc& _Other) = delete;
	ContentFunc(ContentFunc&& _Other) noexcept = delete;
	ContentFunc& operator=(const ContentFunc& _Other) = delete;
	ContentFunc& operator=(ContentFunc&& _Other) noexcept = delete;
};
