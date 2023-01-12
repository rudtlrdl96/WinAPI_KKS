#pragma once


class SubjectManager sealed
{
public:
	SubjectManager();
	~SubjectManager();

	SubjectManager(const SubjectManager& _Other) = delete;
	SubjectManager(SubjectManager&& _Other) noexcept = delete;
	SubjectManager& operator=(const SubjectManager& _Other) = delete;
	SubjectManager& operator=(SubjectManager&& _Other) noexcept = delete;

protected:

private:

};