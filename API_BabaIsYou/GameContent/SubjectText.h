#pragma once
#include "TextActor.h"

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
};