#pragma once
#include <GameEngineBase/GameEngineDebug.h>
#include <vector>
#include "GridActor.h"

class TextActor : public GridActor
{
public:
	TextActor();
	~TextActor();

	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Render();

	TextActor(const TextActor& _Other) = delete;
	TextActor(TextActor&& _Other) noexcept = delete;
	TextActor& operator=(const TextActor& _Other) = delete;
	TextActor& operator=(TextActor&& _Other) noexcept = delete;


protected:

private:

	static std::vector<std::vector<TextActor*>> vecTextGrid;

	static void InitTextGrid(const float4& _Size);
	static void DeleteTextGrid();

	bool Move(const float4& _Dir);
	void Interaction();
};