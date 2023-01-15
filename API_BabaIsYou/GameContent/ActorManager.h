#pragma once
#include <string_view>
#include <vector>

class GameEngineLevel;
class GridActor;
class ActorManager sealed
{
public:
	ActorManager(GameEngineLevel* _PuzzleLevel);
	~ActorManager();

	ActorManager(const ActorManager& _Other) = delete;
	ActorManager(ActorManager&& _Other) noexcept = delete;
	ActorManager& operator=(const ActorManager& _Other) = delete;
	ActorManager& operator=(ActorManager&& _Other) noexcept = delete;
	
	void LoadData(const std::string_view& _PuzzleName);
	void Reset();
protected:

private:
	void Init(GameEngineLevel* _PuzzleLevel);

	std::vector<GridActor*> vecActors;
};