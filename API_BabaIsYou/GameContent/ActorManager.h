#pragma once
#include <string_view>
#include <vector>
#include <list>

class GameEngineLevel;
class GridActor;
class ActorManager sealed
{
public:
	ActorManager(GameEngineLevel* _PuzzleLevel);
	~ActorManager();

	void Update(float _Time);
	void LateUpdate(float _Time);

	ActorManager(const ActorManager& _Other) = delete;
	ActorManager(ActorManager&& _Other) noexcept = delete;
	ActorManager& operator=(const ActorManager& _Other) = delete;
	ActorManager& operator=(ActorManager&& _Other) noexcept = delete;
	
	void LoadData(const std::string_view& _PuzzleName);

	static void ActiveUpdateBehavior()
	{
		UpdateBehavior = true;
	}

protected:

private:
	void Init(GameEngineLevel* _PuzzleLevel);

	std::list<int2> vecWaitInputs;
	std::vector<GridActor*> vecActors;
	static bool UpdateBehavior;

	void clear();
	void Reset();
};