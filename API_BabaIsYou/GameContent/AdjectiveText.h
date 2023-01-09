#pragma once
#include "TextActor.h"

enum ADJECTIVE_TYPE
{
	YOU,
	WIN,
	STOP,
	PUSH,
	SINK,
	DEFEAT,
	HOT,
	MELT,
};

class AdjectiveText : public TextActor
{
public:
	AdjectiveText();
	~AdjectiveText();

	AdjectiveText(const AdjectiveText& _Other) = delete;
	AdjectiveText(AdjectiveText&& _Other) noexcept = delete;
	AdjectiveText& operator=(const AdjectiveText& _Other) = delete;
	AdjectiveText& operator=(AdjectiveText&& _Other) noexcept = delete;

protected:

private:
	ADJECTIVE_TYPE Type = ADJECTIVE_TYPE::YOU;
};