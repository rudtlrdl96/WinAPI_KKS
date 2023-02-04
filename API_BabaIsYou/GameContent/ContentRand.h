#pragma once


class ContentRand
{
public:
	static int RandInt(int _Min, int _Max);
	static float RandFloat(float _Min, float _Max);

protected:

private:
	ContentRand();
	~ContentRand();

	ContentRand(const ContentRand& _Other) = delete;
	ContentRand(ContentRand&& _Other) noexcept = delete;
	ContentRand& operator=(const ContentRand& _Other) = delete;
	ContentRand& operator=(ContentRand&& _Other) noexcept = delete;
};