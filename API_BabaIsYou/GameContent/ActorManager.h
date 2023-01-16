#pragma once
#include <string_view>
#include <vector>
#include <list>

class GameEngineLevel;
class GridActor;
class ActorManager sealed
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
	ActorManager(GameEngineLevel* _PuzzleLevel);
	~ActorManager();

	void Input(float _DT);

	ActorManager(const ActorManager& _Other) = delete;
	ActorManager(ActorManager&& _Other) noexcept = delete;
	ActorManager& operator=(const ActorManager& _Other) = delete;
	ActorManager& operator=(ActorManager&& _Other) noexcept = delete;

	void clear();
	void LoadData(const std::string_view& _PuzzleName);

	static void ActiveUpdateBehavior()
	{
		UpdateBehavior = true;
	}

protected:

private:
	void Init(GameEngineLevel* _PuzzleLevel);

	std::list<INPUTBEHAVIOR> vecWaitInputs;
	std::vector<GridActor*> vecActors;
	static bool UpdateBehavior;

	void Reset();
};