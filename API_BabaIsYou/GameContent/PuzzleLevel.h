#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <string>
#include <string_view>

class FadeUI;
class CongratulationsUI;
class GridActorManager;
class TutorialUI;
class CameraSystem;
class SoundSystem;
class PuzzleLevel : public GameEngineLevel
{
public:
	PuzzleLevel();
	~PuzzleLevel();

	PuzzleLevel(const PuzzleLevel& _Other) = delete;
	PuzzleLevel(PuzzleLevel&& _Other) noexcept = delete;
	PuzzleLevel& operator=(const PuzzleLevel& _Other) = delete;
	PuzzleLevel& operator=(PuzzleLevel&& _Other) noexcept = delete;

	static void PuzzleLevelExit();
	static void SetPuzzleMapName(const std::string_view& _MapName);
	static void SetPuzzleMapInfo(const std::string_view& _MapInfo);
	static void SetPuzzleMapLevel(size_t _Level);
	void Restart();
	void LoadPuzzleData();

	inline CameraSystem* GetCameraSystem() const
	{
		return CameraSystemPtr;
	}

protected:
	void Loading() override;
	void Update(float _DT) override;

	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override;
private:
	static std::string LoadPuzzleName;
	static std::string LoadPuzzleInfo;
	static size_t LevelNumber;

	FadeUI* PuzzleFadeActor = nullptr;
	CongratulationsUI* CongratulationActor = nullptr;
	CameraSystem* CameraSystemPtr = nullptr;
	SoundSystem* SoundSystemPtr = nullptr;
	TutorialUI* TutorialRender = nullptr;

	static bool IsExitValue;
};