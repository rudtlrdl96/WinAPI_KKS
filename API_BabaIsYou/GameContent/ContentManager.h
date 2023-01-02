#pragma once


class ContentManager
{
public:
	ContentManager();
	~ContentManager();

	ContentManager(const ContentManager& _Other) = delete;
	ContentManager(ContentManager&& _Other) noexcept = delete;
	ContentManager& operator=(const ContentManager& _Other) = delete;
	ContentManager& operator=(ContentManager&& _Other) noexcept = delete;

protected:

private:

};