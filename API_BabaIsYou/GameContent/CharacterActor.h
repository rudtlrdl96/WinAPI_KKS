#pragma once
#include "SubjectActor.h"

class CharacterActor : public SubjectActor
{
public:
	CharacterActor();
	~CharacterActor();

	CharacterActor(const CharacterActor& _Other) = delete;
	CharacterActor(CharacterActor&& _Other) noexcept = delete;
	CharacterActor& operator=(const CharacterActor& _Other) = delete;
	CharacterActor& operator=(CharacterActor&& _Other) noexcept = delete;

protected:

private:

};