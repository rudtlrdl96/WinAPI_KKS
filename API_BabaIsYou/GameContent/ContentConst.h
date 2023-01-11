#pragma once


class ContentConst
{
public:
	static const int WIGGLE_MAXTICK;
	static const float WIGGLE_TIME;
public:
	ContentConst(const ContentConst& _Other) = delete;
	ContentConst(ContentConst&& _Other) noexcept = delete;
	ContentConst& operator=(const ContentConst& _Other) = delete;
	ContentConst& operator=(ContentConst&& _Other) noexcept = delete;

protected:

private:
	ContentConst();
	~ContentConst();
};