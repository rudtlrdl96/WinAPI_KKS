#include "StringUI.h"
#include <math.h>
#include <cmath>
#include <GameEngineCore/GameEngineLevel.h>
#include <time.h>
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

	for (int i = 0; i < _Text.size(); ++i)
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
			CreateTextActor(_Text[i]);
		}
		else
		{
			vecTextActors[ActiveActorCount]->SetText(_Text[i]);
		}

		LineDistanceX += GetFontInterval().x;
		FontPos.x += GetFontInterval().x;

		vecTextActors[ActiveActorCount]->SetScale(GetFontSize());
		vecTextActors[ActiveActorCount]->SetPos(FontPos);
		vecTextActors[ActiveActorCount]->On();

		++LineEndIndex;
		++ActiveActorCount;
	}

	LineOrderX(LineStartIndex, LineEndIndex, LineDistanceX);
}

void StringUI::StringOn()
{
	for (size_t i = 0; i < ActiveActorCount; i++)
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
		vecTextActors[i]->ResetShake();
	}

	Off();
}

void StringUI::SetStringColor(TEXT_COLOR _Color)
{
	for (size_t i = 0; i < vecTextActors.size(); i++)
	{
		vecTextActors[i]->SetTextColor(_Color);
	}
}

void StringUI::LerpScaleString(float4 _StartSize, float4 _EndSize, float _Time, bool IsEndOff)
{
	for (size_t i = 0; i < ActiveActorCount; i++)
	{
		vecTextActors[i]->LerpSize(_StartSize, _EndSize, _Time, IsEndOff);
	}
}

void StringUI::RandShakeString(float _Distance, float _Time)
{
	std::srand(static_cast<int>(time(nullptr)));

	for (size_t i = 0; i < ActiveActorCount; i++)
	{
		vecTextActors[i]->ResetShake();
		float4 RandDir;
		float RandRot = 360.0f / (std::rand() % 360);

		RandDir.x = std::cos(RandRot);
		RandDir.y = std::sin(RandRot);

		vecTextActors[i]->ShakeText(RandDir * _Distance, _Time);
	}
}

void StringUI::CreateTextActor(char _Text)
{
	TextUI* ReturnText = GetLevel()->CreateActor<TextUI>();
	ReturnText->Init(TextRenderOrder);
	ReturnText->SetText(_Text);
	vecTextActors.push_back(ReturnText);
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