#pragma once
#include <chrono>
#include <Windows.h>

// 시간을 재거나 하는 기능들을 담당합니다.
// 기본적으로 시간을 재는 방법은 여러가지가 있지만
// 선생님은 C++ std의 최신기능인 크로노를 이용할겁니다.
// 시간을 재는 기능은 기본적으로 CPU에 별개로 들어가 있습니다.
// 그 하드웨어에 os가 값을 받아와서 우리에게 알려주는 개념이다.

// 그래서 기본적으로 시간을 재면 정수로 표현하게 됩니다.
// 기준을 잡습니다. 

// 설명 :
class GameEngineTime
{
public:
	// constrcuter destructer
	GameEngineTime();
	~GameEngineTime();

	// delete Function
	GameEngineTime(const GameEngineTime& _Other) = delete;
	GameEngineTime(GameEngineTime&& _Other) noexcept = delete;
	GameEngineTime& operator=(const GameEngineTime& _Other) = delete;
	GameEngineTime& operator=(GameEngineTime&& _Other) noexcept = delete;

	void TimeCheckStart();

	// 이 사이에 벌어진 일의 시간을 재는 기능입니다.

	float TimeCheckEnd();

protected:


private:
	LARGE_INTEGER Prev = LARGE_INTEGER();
	LARGE_INTEGER Current = LARGE_INTEGER();
	LARGE_INTEGER Second = LARGE_INTEGER();


};

