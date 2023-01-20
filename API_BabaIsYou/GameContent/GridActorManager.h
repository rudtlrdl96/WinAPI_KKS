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
	static GridActorManager* GetInst()
	{
		static GridActorManager Instance;
		return &Instance;
	}

	void Init(GameEngineLevel* _PuzzleLevel);
	void Input(float _DT);

	void clear();
	void LoadData(const std::string_view& _PuzzleName);

	void ActiveUpdateBehavior()
	{
		UpdateBehavior = true;
	}

	bool IsPuzzleEnd() const;

protected:

private:
	GridActorManager();
	~GridActorManager();

	GridActorManager(const GridActorManager& _Other) = delete;
	GridActorManager(GridActorManager&& _Other) noexcept = delete;
	GridActorManager& operator=(const GridActorManager& _Other) = delete;
	GridActorManager& operator=(GridActorManager&& _Other) noexcept = delete;

	bool UpdateBehavior = false;

	std::list<INPUTBEHAVIOR> vecWaitInputs;

	GameEngineLevel* PuzzleLevel = nullptr;
	BlackBackUI* GridBackActor = nullptr;

	void Reset();
};