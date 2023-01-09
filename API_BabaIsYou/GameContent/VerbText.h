#pragma once
#include "TextActor.h"

enum VERB_TYPE
{
	IS,
	AND,
};

class VerbText : public TextActor
{
public:
	VerbText();
	~VerbText();

	VerbText(const VerbText& _Other) = delete;
	VerbText(VerbText&& _Other) noexcept = delete;
	VerbText& operator=(const VerbText& _Other) = delete;
	VerbText& operator=(VerbText&& _Other) noexcept = delete;

protected:

private:
	VERB_TYPE Type = VERB_TYPE::IS;
};