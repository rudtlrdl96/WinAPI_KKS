#pragma once
#include <vector>
#include <GameEngineCore/GameEngineActor.h>

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

protected:
	void Start() override;
	void Update(float _DT) override;
private:
	
	std::vector<PalletButton*> vecLoadActors;
	ButtonUI* CloseButton = nullptr;
	PalletSelect* PalletSelectActor = nullptr;

	int PenEnum = -1;

	void SelectPen(size_t _Index);

};