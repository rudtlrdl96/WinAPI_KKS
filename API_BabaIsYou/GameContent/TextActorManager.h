#pragma once


class TextActorManager
{
public:
	TextActorManager();
	~TextActorManager();

	TextActorManager(const TextActorManager& _Other) = delete;
	TextActorManager(TextActorManager&& _Other) noexcept = delete;
	TextActorManager& operator=(const TextActorManager& _Other) = delete;
	TextActorManager& operator=(TextActorManager&& _Other) noexcept = delete;

protected:

private:

};