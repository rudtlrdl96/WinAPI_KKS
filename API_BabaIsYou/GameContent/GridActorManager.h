#pragma once
#include <string_view>
#include <vector>
#include <list>

class GameEngineLevel;
class GridActor;
class BlackBackUI;
class GridActorManager sealed
{
private:
	enum class INPUTBEHAVIOR
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		UNDO,
		WAIT
	};
public:
	GridActorManager(GameEngineLevel* _PuzzleLevel);
	~GridActorManager();

	void Input(float _DT);

	GridActorManager(const GridActorManager& _Other) = delete;
	GridActorManager(GridActorManager&& _Other) noexcept = delete;
	GridActorManager& operator=(const GridActorManager& _Other) = delete;
	GridActorManager& operator=(GridActorManager&& _Other) noexcept = delete;

	void clear();
	void LoadData(const std::string_view& _PuzzleName);

	static void ActiveUpdateBehavior()
	{
		UpdateBehavior = true;
	}

	bool IsPuzzleEnd() const;

protected:

private:
	static bool UpdateBehavior;

	std::list<INPUTBEHAVIOR> vecWaitInputs;
	std::vector<GridActor*> vecActors;

	GameEngineLevel* PuzzleLevel = nullptr;
	BlackBackUI* GridBackActor = nullptr;

	void Init(GameEngineLevel* _PuzzleLevel);
	void Reset();
};