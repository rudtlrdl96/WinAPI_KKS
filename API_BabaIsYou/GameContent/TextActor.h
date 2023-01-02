#pragma once


class TextActor
{
public:
	TextActor();
	~TextActor();

	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Render();

	TextActor(const TextActor& _Other) = delete;
	TextActor(TextActor&& _Other) noexcept = delete;
	TextActor& operator=(const TextActor& _Other) = delete;
	TextActor& operator=(TextActor&& _Other) noexcept = delete;

protected:

private:

};