#pragma once
#include <map>

class GridActor;
class RuleManager
{
public:
	static RuleManager* GetInst()
	{
		static RuleManager Instance;
		return &Instance;
	}

	void RuleCheck();
	void RuleReset();

protected:

private:
	RuleManager();
	~RuleManager();

	RuleManager(const RuleManager& _Other) = delete;
	RuleManager(RuleManager&& _Other) noexcept = delete;
	RuleManager& operator=(const RuleManager& _Other) = delete;
	RuleManager& operator=(RuleManager&& _Other) noexcept = delete;
};