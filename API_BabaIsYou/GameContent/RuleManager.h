#pragma once
#include <map>

class GridActor;
class RuleManager
{
public:
	enum class RULE_TYPE
	{
		SUBJECT,
		DEFINE,
	};

	class Rule
	{
	public:
		friend class RuleManager;

		union RuleValue
		{
			struct
			{
				char RuleType;
				char VerbValue;
				char DefineValue;
			};

			int KeyValue = INT32_MIN;
		};
	public:
		RuleValue Value;

	private:
		int RuleCount = 0;
	};

public:
	static RuleManager* GetInst()
	{
		static RuleManager Instance;
		return &Instance;
	}

	void AddRule(const Rule& _Rule);
	void RemoveRule(const Rule& _Rule);
	void RuleReset();

	const std::map<int, Rule>& GetRules() const
	{
		return mapRules;
	}

protected:

private:
	RuleManager();
	~RuleManager();

	RuleManager(const RuleManager& _Other) = delete;
	RuleManager(RuleManager&& _Other) noexcept = delete;
	RuleManager& operator=(const RuleManager& _Other) = delete;
	RuleManager& operator=(RuleManager&& _Other) noexcept = delete;

	std::map<int, Rule> mapRules;
};