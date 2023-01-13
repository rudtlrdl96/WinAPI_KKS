#pragma once
#include <map>

// Rule �̶�� ����
// Rule �� �־� + ���� + �־� or �־� + ���� + ����� �ռ��� ���� �� �ִ�
// And�� is �ڿ� ������ is �տ��ִ� ��ü ���� is�� �ȴ�
// is �� �������� ���Ǹ� �־ �����Ѵٴ� ������ �Ǿ� �Ѵ�

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