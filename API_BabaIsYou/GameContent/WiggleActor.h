#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineResources.h>

class WiggleActor : public GameEngineActor
{
public:
	WiggleActor();
	~WiggleActor();

	WiggleActor(const WiggleActor& _Other) = delete;
	WiggleActor(WiggleActor&& _Other) noexcept = delete;
	WiggleActor& operator=(const WiggleActor& _Other) = delete;
	WiggleActor& operator=(WiggleActor&& _Other) noexcept = delete;

	static size_t GetWiggleIndex()
	{
		return WiggleIndex;
	}

protected:

private:
	static size_t WiggleIndex;
	static void Wiggle();

	friend class WiggleManager;
};