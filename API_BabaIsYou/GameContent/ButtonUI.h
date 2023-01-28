#pragma once
#include <GameEngineCore/GameEngineActor.h>

class GameEngineRender;
class ButtonUI : public GameEngineActor
{
public:
	ButtonUI();
	~ButtonUI();

	ButtonUI(const ButtonUI& _Other) = delete;
	ButtonUI(ButtonUI&& _Other) noexcept = delete;
	ButtonUI& operator=(const ButtonUI& _Other) = delete;
	ButtonUI& operator=(ButtonUI&& _Other) noexcept = delete;

	inline void SetColSize(const float4& _Size)
	{
		ColSize = _Size;
	}

	inline void SetColOffsetSize(const float4& _Size)
	{
		ColOffset = _Size;
	}

	void SetImage(const std::string_view& _ImageName, float4 _Scale);

	inline bool IsUp() const
	{
		return IsUpValue;
	}
protected:
	void Start() override;
	void Update(float _DT) override;
	void Render(float _DT) override;
private:
	float4 ColSize = float4::Zero;
	float4 ColOffset = float4::Zero;

	bool IsOverlapValue = false;
	bool IsClickValue = false;
	bool IsUpValue = false;

	GameEngineRender* ImageRender = nullptr;
};