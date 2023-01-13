#pragma once

enum RENDER_ORDER
{
	BACKGROUND,
	WORLDMAP,
	BABA,
	KEKE,
	TEXT,
	UIIMAGE,
	BUTTON,
	FADE,
};

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