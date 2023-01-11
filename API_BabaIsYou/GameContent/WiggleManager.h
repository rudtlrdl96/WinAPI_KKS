#pragma once

class WiggleManager
{
private:
	WiggleManager();
	~WiggleManager();
public:

	static void Update();

	WiggleManager(const WiggleManager& _Other) = delete;
	WiggleManager(WiggleManager&& _Other) noexcept = delete;
	WiggleManager& operator=(const WiggleManager& _Other) = delete;
	WiggleManager& operator=(WiggleManager&& _Other) noexcept = delete;

protected:

private:
	static float PrograssTime;

};