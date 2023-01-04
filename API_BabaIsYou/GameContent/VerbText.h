#pragma once
#include "TextActor.h"

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

};