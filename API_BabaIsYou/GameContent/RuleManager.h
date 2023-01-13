#pragma once
#include <map>

// Rule 이라는 개념
// Rule 은 주어 + 동사 + 주어 or 주어 + 동사 + 목적어를 합성해 만들 수 있다
// And는 is 뒤에 붙으면 is 앞에있던 객체 기준 is가 된다
// is 가 오기전에 사용되면 주어를 포함한다는 개념이 되야 한다

 class RuleManager sealed
{
private:
	union RuleKey
	{
		struct
		{
			char TextKey[8];
		};

		__int64 Key;
	};
public:
	class Rule
	{
	public:

	};

public:
	RuleManager();
	~RuleManager();

	void UpdateRules();
	bool AddRules(int a, int b, int c);

	RuleManager(const RuleManager& _Other) = delete;
	RuleManager(RuleManager&& _Other) noexcept = delete;
	RuleManager& operator=(const RuleManager& _Other) = delete;
	RuleManager& operator=(RuleManager&& _Other) noexcept = delete;

protected:

private:
	std::map<RuleKey, int> mapRules;
};