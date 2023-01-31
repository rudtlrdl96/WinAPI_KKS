#pragma once
#include <vector>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

class ButtonUI;
class PalletButton;
class PalletSelect;
class GameEngineRender;
class PalletActor : public GameEngineActor
{
public:
	PalletActor();
	~PalletActor();

	PalletActor(const PalletActor& _Other) = delete;
	PalletActor(PalletActor&& _Other) noexcept = delete;
	PalletActor& operator=(const PalletActor& _Other) = delete;
	PalletActor& operator=(PalletActor&& _Other) noexcept = delete;
	
	void ActivePallet();
	void DisablePallet();

	inline int GetPenEnum() const
	{
		return PenEnum;
	}

	inline DIR_FLAG GetPalletDir() const
	{
		return PalletActorDir;
	}

protected:
	void Start() override;
	void Update(float _DT) override;
private:
	
	std::vector<PalletButton*> vecLoadActors;
	PalletSelect* PalletSelectActor = nullptr;

	ButtonUI* LeftDirButton = nullptr;
	ButtonUI* RightDirButton = nullptr;
	ButtonUI* UpDirButton = nullptr;
	ButtonUI* DownDirButton = nullptr;

	int PenEnum = -1;
	DIR_FLAG PalletActorDir = DIR_FLAG::RIGHT;

	void SelectPen(size_t _Index);
	void SetDir(DIR_FLAG _Dir);
};