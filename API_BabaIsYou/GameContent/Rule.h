#pragma once

class GridActor;
class Rule
{
public:
	Rule(int _RuleKey);
	~Rule();

	Rule(const Rule& _Other) = delete;
	Rule(Rule&& _Other) noexcept = delete;
	Rule& operator=(const Rule& _Other) = delete;
	Rule& operator=(Rule&& _Other) noexcept = delete;

	static void CreateRule(GridActor* _Actor);

	void ActiveRule();
	void DisableRule();

protected:

private:
	const int RuleKey = -1;

	GridActor* SubjectActor = nullptr;
	GridActor* VerbActor = nullptr;
	GridActor* DefineActor = nullptr;

};