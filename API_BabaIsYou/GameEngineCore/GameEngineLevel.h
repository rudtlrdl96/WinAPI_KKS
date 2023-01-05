#pragma once
#include <list>

// 설명 :
class GameEngineCore;
class GameEngineActor;
class GameEngineLevel
{
	friend GameEngineCore;

public:
	// constrcuter destructer
	GameEngineLevel();
	virtual ~GameEngineLevel() = 0;

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	void CreateActor()
	{
		GameEngineActor* Actor = new ActorType();

		ActorStart(Actor);

		Actors.push_back(Actor);
	}

protected:
	virtual void Loading() = 0;
	virtual void Update() = 0;

private:
	// 컨텐츠를 알아서도 안되지만
	//std::list<Player*> Actors;
	//std::list<Monster*> Actors;
	//std::list<Background*> Actors;

	// 하나의 자료형으로 모든 화면내에 등장하는 것들을 표현할수 있게 됩니다.
	std::list<GameEngineActor*> Actors;

	void ActorsUpdate();
	void ActorsRender();


	void ActorStart(GameEngineActor* _Actor);

};

