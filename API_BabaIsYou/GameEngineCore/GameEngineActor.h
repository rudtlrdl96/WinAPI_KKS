#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <Windows.h>

// 화면에 존재하고 위치가 있어야하는 모든것들의 기본기능을 지원해줄 겁니다.
// 그려져야 한다.

// 설명 :
class GameEngineLevel;
class GameEngineActor
{
	friend GameEngineLevel;

public:
	// constrcuter destructer
	GameEngineActor();
	~GameEngineActor();

	// delete Function
	GameEngineActor(const GameEngineActor& _Other) = delete;
	GameEngineActor(GameEngineActor&& _Other) noexcept = delete;
	GameEngineActor& operator=(const GameEngineActor& _Other) = delete;
	GameEngineActor& operator=(GameEngineActor&& _Other) noexcept = delete;

	float4 GetPos()
	{
		return Pos;
	}

	void SetPos(const float4& _MovePos)
	{
		Pos = _MovePos;
	}

	void SetMove(const float4& _MovePos)
	{
		Pos += _MovePos;
	}


protected:
	// 안구현할수도 있다.
	// ex) 나무는 Update를 안구현할수도 있다.

	// 시작하기전에 뭔가 준비해야할것이 있으면 여기서 해라.
	virtual void Start() {}

	// 키입력을 받거나 인공지능을 점수를 계산하거나 하는 것들을 여기서 처리
	virtual void Update() {}

	// 화면에 그려지는 기능들을 여기서 처리
	virtual void Render() {}

private:
	float4 Pos = { 0.0f, 0.0f };
};

