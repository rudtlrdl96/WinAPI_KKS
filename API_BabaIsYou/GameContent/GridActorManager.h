#pragma once
#include <string_view>
#include <vector>
#include <list>

#include "ContentEnum.h"

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

	class RemoveDefineData
	{
	public:
		GridActor* ActorData = nullptr;
		ACTOR_DEFINE RemoveDefine = ACTOR_DEFINE::NONE;
	};
public:
	static GridActorManager* GetInst()
	{
		static GridActorManager Instance;
		return &Instance;
	}

	void Init(GameEngineLevel* _PuzzleLevel);
	void Input(float _DT);

	void AddRemoveDefine(GridActor* _Actor, ACTOR_DEFINE _Define);
	void clear();
	void LoadData(const std::string_view& _PuzzleName);
	bool IsPuzzleEnd() const;

protected:

private:

	GridActorManager();
	~GridActorManager();

	GridActorManager(const GridActorManager& _Other) = delete;
	GridActorManager(GridActorManager&& _Other) noexcept = delete;
	GridActorManager& operator=(const GridActorManager& _Other) = delete;
	GridActorManager& operator=(GridActorManager&& _Other) noexcept = delete;
		
	std::vector<RemoveDefineData> vecDefineRemoveActors;
	std::list<INPUTBEHAVIOR> listInputBuffer;

	GameEngineLevel* PuzzleLevel = nullptr;
	BlackBackUI* GridBackActor = nullptr;

	void Reset();
};