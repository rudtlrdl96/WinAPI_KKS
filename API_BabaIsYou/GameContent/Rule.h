#pragma once
#include "ContentEnum.h"
#include "ContentMath.h"

class PuzzleActor;
class RuleManager;
class Rule
{
	friend RuleManager;
public:
	Rule(int _Key);
	~Rule();

	Rule(const Rule& _Other) = delete;
	Rule(Rule&& _Other) noexcept = delete;
	Rule& operator=(const Rule& _Other) = delete;
	Rule& operator=(Rule&& _Other) noexcept = delete;

	static void CreateRule(PuzzleActor* _Actor, bool _IsInit = false);
	static void RemoveRule(PuzzleActor* _Actor);

	const int RuleKey = -1;
protected:

private:
	PuzzleActor* SubjectActor = nullptr;
	PuzzleActor* VerbActor = nullptr;
	PuzzleActor* DefineActor = nullptr;

	static void CheckRule(PuzzleActor* _SubjectActor, PuzzleActor* _VerbActor, PuzzleActor* _DefineActor, bool _IsInit);
};