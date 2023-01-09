#pragma once
#include "GridActor.h"
#include <vector>
#include <list>

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
	static std::vector<std::vector<std::list<SubjectActor*>>> vecTextGrid;

	static void InitTextGrid(const float4& _Size);
	static void DeleteTextGrid();


	bool Move(const float4& _Dir) override;
};