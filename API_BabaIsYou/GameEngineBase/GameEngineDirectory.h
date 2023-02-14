#pragma once
#include <string_view>
#include <vector>
#include "GameEnginePath.h"


// Ό³Έν :
class GameEngineFile;
class GameEngineDirectory
{
public:
	GameEngineDirectory();
	~GameEngineDirectory();

	GameEngineDirectory(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory(GameEngineDirectory&& _Other) noexcept = delete;
	GameEngineDirectory& operator=(const GameEngineDirectory& _Other) = delete;
	GameEngineDirectory& operator=(GameEngineDirectory&& _Other) noexcept = delete;

	bool MoveParent();
	void MoveParentToDirectory(const std::string_view& _String);
	bool Move(const std::string_view& _String);

	GameEnginePath GetPlusFileName(const std::string_view& _String);

protected:

private:
	GameEnginePath Path;
};

