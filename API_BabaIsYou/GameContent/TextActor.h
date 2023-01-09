#pragma once
#include <GameEngineBase/GameEngineDebug.h>
#include <vector>
#include "GridActor.h"

class TextActor : public GridActor
{
public:
	TextActor();
	~TextActor();

	TextActor(const TextActor& _Other) = delete;
	TextActor(TextActor&& _Other) noexcept = delete;
	TextActor& operator=(const TextActor& _Other) = delete;
	TextActor& operator=(TextActor&& _Other) noexcept = delete;


protected:

private:
};