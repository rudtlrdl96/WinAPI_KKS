#pragma once
#include "WiggleActor.h"

class PalletSelect : public WiggleActor
{
public:
	PalletSelect();
	~PalletSelect();

	PalletSelect(const PalletSelect& _Other) = delete;
	PalletSelect(PalletSelect&& _Other) noexcept = delete;
	PalletSelect& operator=(const PalletSelect& _Other) = delete;
	PalletSelect& operator=(PalletSelect&& _Other) noexcept = delete;

protected:
	void Start() override;
private:

};