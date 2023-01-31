#pragma once
#include <string_view>
#include <vector>
#include <list>

#include "ContentEnum.h"

class PuzzleLevel;
class PuzzleActor;
class BlackBackUI;
class PuzzleActorManager sealed
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

	class DefineData
	{
	public:
		PuzzleActor* ActorData = nullptr;
		ACTOR_DEFINE RemoveDefine = ACTOR_DEFINE::NONE;

		bool IsRemove = false;
	};
public:
	static PuzzleActorManager* GetInst()
	{
		static PuzzleActorManager Instance;
		return &Instance;
	}

	void Init(PuzzleLevel* _PuzzleLevel);
	void Reset();
	void Input(float _DT);

	void AddDefine(PuzzleActor* _Actor, ACTOR_DEFINE _Define, bool _IsRemove);
	void clear();
	void LoadData(const std::string_view& _PuzzleName);
	bool IsPuzzleEnd() const;

protected:

private:

	PuzzleActorManager();
	~PuzzleActorManager();

	PuzzleActorManager(const PuzzleActorManager& _Other) = delete;
	PuzzleActorManager(PuzzleActorManager&& _Other) noexcept = delete;
	PuzzleActorManager& operator=(const PuzzleActorManager& _Other) = delete;
	PuzzleActorManager& operator=(PuzzleActorManager&& _Other) noexcept = delete;
		
	std::vector<DefineData> vecDefineActors;
	std::list<INPUTBEHAVIOR> listInputBuffer;

	PuzzleLevel* MainPuzzleLevel = nullptr;
	BlackBackUI* GridBackActor = nullptr;

};