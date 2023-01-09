#pragma once
#include "TextActor.h"

enum TEXT_SUBJECT_TYPE
{
	BABA,
	KEKE,
	ROBOT,
	WALL,
	WATER,
	LAVA,
	GRASS,
	BRICK,
	PLANK,
	FLAG,
	ROCK,
	SKULL,
	FLOWER
};

class SubjectText : public TextActor
{
public:
	SubjectText();
	~SubjectText();

	SubjectText(const SubjectText& _Other) = delete;
	SubjectText(SubjectText&& _Other) noexcept = delete;
	SubjectText& operator=(const SubjectText& _Other) = delete;
	SubjectText& operator=(SubjectText&& _Other) noexcept = delete;

protected:

private:
	TEXT_SUBJECT_TYPE Type = TEXT_SUBJECT_TYPE::BABA;
};