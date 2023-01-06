#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <string>
#include <string_view>

class PuzzleLevel : public GameEngineLevel
{
public:
	PuzzleLevel();
	~PuzzleLevel();

	PuzzleLevel(const PuzzleLevel& _Other) = delete;
	PuzzleLevel(PuzzleLevel&& _Other) noexcept = delete;
	PuzzleLevel& operator=(const PuzzleLevel& _Other) = delete;
	PuzzleLevel& operator=(PuzzleLevel&& _Other) noexcept = delete;

	void LoadPuzzleData(const std::string_view& _PuzzleName);

protected:
	void Loading() override;
	void Update() override;
private:
	static std::string LoadPuzzleName;
};