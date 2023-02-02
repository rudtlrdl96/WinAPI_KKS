#include "StringUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>
#include "TextUI.h"
#include "ContentEnum.h"

StringUI::StringUI() :
	TextRenderOrder(static_cast<int>(RENDER_ORDER::TEXTUI))
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

	float LineDistanceX = 0.0f;
	int LineStartIndex = 0;
	int LineEndIndex = 0;

	for (size_t i = 0; i < _Text.size(); ++i)
	{
		if (_Text[i] == '\n')
		{
			LineOrderX(LineStartIndex, LineEndIndex, LineDistanceX);

			FontPos.x = GetPos().x;
			FontPos.y += GetFontInterval().y;

			LineDistanceX = 0.0f;
			LineStartIndex = LineEndIndex;

			continue;
		}

		if (_Text[i] == '\t')
		{
			FontPos.x += FontInterval.x * 4;
			LineDistanceX += FontInterval.x * 4;
			continue;
		}

		if (vecTextActors.size() <= ActiveActorCount)
		{
			vecTextActors.push_back(CreateTextActor(_Text[i]));
		}

		LineDistanceX += GetFontInterval().x;
		FontPos.x += GetFontInterval().x;

		vecTextActors[ActiveActorCount]->SetText(_Text[i]);
		vecTextActors[ActiveActorCount]->SetPos(FontPos);
		vecTextActors[ActiveActorCount]->SetScale(GetFontSize());
		vecTextActors[ActiveActorCount]->On();

		++LineEndIndex;
		++ActiveActorCount;
	}

	LineOrderX(LineStartIndex, LineEndIndex, LineDistanceX);
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
	TextUI* ReturnText = GetLevel()->CreateActor<TextUI>();
	ReturnText->Init(TextRenderOrder);
	return ReturnText;
}


void StringUI::LineOrderX(int _Start, int _End, float _Distance)
{
	if (0 > _Start || 0 > _End)
	{
		MsgAssert("잘못된 StringUI 인덱스 접근입니다");
		return;
	}

	if (vecTextActors.size() <= _Start || vecTextActors.size() < _End)
	{
		MsgAssert("잘못된 StringUI 인덱스 접근입니다");
		return;
	}

	for (int i = _Start; i < _End; i++)
	{
		switch (TextOrder)
		{
		case StringUI::TEXT_ORDER::MIDDLE:
			vecTextActors[i]->SetMove(-float4{ _Distance * 0.5f, 0.0f });
			break;
		case StringUI::TEXT_ORDER::RIGHT:
			vecTextActors[i]->SetMove(-float4{ _Distance, 0.0f });
			break;
		}
	}
}