#include "StringUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>
#include "TextUI.h"

StringUI::StringUI()
{
}

StringUI::~StringUI()
{
}

void StringUI::WriteText(const std::string_view& _Text)
{
	if (vecTextActors.capacity() < _Text.size())
	{
		vecTextActors.reserve(_Text.size());
	}

	StringOff();
	ActiveActorCount = 0;
	float4 FontPos = GetPos();

	for (size_t i = 0; i < _Text.size(); ++i)
	{
		if (_Text[i] == '\n')
		{
			FontPos.x = GetPos().x;
			FontPos.y += GetFontInterval().y;
			continue;
		}

		if (vecTextActors.size() <= ActiveActorCount)
		{
			vecTextActors.push_back(CreateTextActor(_Text[i]));
		}

		FontPos.x += GetFontInterval().x;

		vecTextActors[ActiveActorCount]->SetText(_Text[i]);
		vecTextActors[ActiveActorCount]->SetPos(FontPos);
		vecTextActors[ActiveActorCount]->SetScale(GetFontSize());
		vecTextActors[ActiveActorCount]->On();
		++ActiveActorCount;
	}
}

void StringUI::StringOn()
{
	for (size_t i = 0; i < vecTextActors.size(); i++)
	{
		vecTextActors[i]->On();
	}

	On();
}

void StringUI::StringOff()
{
	for (size_t i = 0; i < vecTextActors.size(); i++)
	{
		vecTextActors[i]->Off();
	}

	Off();
}

TextUI* StringUI::CreateTextActor(char _Text)
{
	return GetLevel()->CreateActor<TextUI>();
}
