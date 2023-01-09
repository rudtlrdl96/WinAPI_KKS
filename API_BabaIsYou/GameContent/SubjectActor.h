#pragma once
#include "GridActor.h"

class SubjectActor : public GridActor
{
public:
	SubjectActor();
	~SubjectActor();

	SubjectActor(const SubjectActor& _Other) = delete;
	SubjectActor(SubjectActor&& _Other) noexcept = delete;
	SubjectActor& operator=(const SubjectActor& _Other) = delete;
	SubjectActor& operator=(SubjectActor&& _Other) noexcept = delete;

protected:

private:
};