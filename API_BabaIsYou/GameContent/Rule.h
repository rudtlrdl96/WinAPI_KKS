#pragma once

class GridActor;
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

	static void CreateRule(GridActor* _Actor, bool _IsInit = false);
	static void RemoveRule(GridActor* _Actor);	

	const int RuleKey = -1;
protected:

private:
	GridActor* SubjectActor = nullptr;
	GridActor* VerbActor = nullptr;
	GridActor* DefineActor = nullptr;

	static void CheckRule(GridActor* _SubjectActor, GridActor* _VerbActor, GridActor* _DefineActor, bool _IsInit);
};