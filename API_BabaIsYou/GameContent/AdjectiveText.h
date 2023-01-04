#pragma once
#include "TextActor.h"

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

};