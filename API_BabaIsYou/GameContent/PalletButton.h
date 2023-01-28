#pragma once
#include "WiggleActor.h"

class GameEngineRender;
class PalletButton : public WiggleActor
{
public:
	PalletButton();
	~PalletButton();

	PalletButton(const PalletButton& _Other) = delete;
	PalletButton(PalletButton&& _Other) noexcept = delete;
	PalletButton& operator=(const PalletButton& _Other) = delete;
	PalletButton& operator=(PalletButton&& _Other) noexcept = delete;

	inline void SetColSize(const float4& _Size)
	{
		ColSize = _Size;
	}

	inline void SetColOffsetSize(const float4& _Size)
	{
		ColOffset = _Size;
	}

	inline bool IsUp() const
	{
		return IsUpValue;
	}

	inline int GetActorEnum() const
	{
		return ActorEnumIndex;
	}

	void SetActorEnum(int _Enum);
	void SetActorImageFrame(int _Frame);

protected:
	void Start() override;
	void Update(float _DT) override;
private:
	float4 ColSize = float4::Zero;
	float4 ColOffset = float4::Zero;

	bool IsOverlapValue = false;
	bool IsClickValue = false;
	bool IsUpValue = false;
	
	GameEngineRender* ActorRender = nullptr;
	int ActorEnumIndex = -1;
	int ActorRenderIndex = -1;
	bool IsEmptyValue = true;
};