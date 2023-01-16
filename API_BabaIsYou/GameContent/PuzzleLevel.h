#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <string>
#include <string_view>

class ActorManager;
class PuzzleLevel : public GameEngineLevel
{
public:
	PuzzleLevel();
	~PuzzleLevel();

	PuzzleLevel(const PuzzleLevel& _Other) = delete;
	PuzzleLevel(PuzzleLevel&& _Other) noexcept = delete;
	PuzzleLevel& operator=(const PuzzleLevel& _Other) = delete;
	PuzzleLevel& operator=(PuzzleLevel&& _Other) noexcept = delete;

	static void SetPuzzleMapName(const std::string_view& _MapName);
	void LoadPuzzleData();

protected:
	void Loading() override;
	void Update(float _DT) override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}
private:
	static std::string LoadPuzzleName;

	ActorManager* ActorMgr = nullptr;
};