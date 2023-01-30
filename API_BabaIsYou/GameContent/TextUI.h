#pragma once


class TextUI
{
public:
	TextUI();
	~TextUI();

	TextUI(const TextUI& _Other) = delete;
	TextUI(TextUI&& _Other) noexcept = delete;
	TextUI& operator=(const TextUI& _Other) = delete;
	TextUI& operator=(TextUI&& _Other) noexcept = delete;

protected:

private:

};