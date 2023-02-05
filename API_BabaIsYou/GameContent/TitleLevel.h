#pragma once
#include <vector>
#include <GameEngineCore/GameEngineLevel.h>

class FadeUI;
class ButtonUI;
class WiggleGridActor;
class WiggleMapToolActor;
class TitleLevel : public GameEngineLevel
{
private:
	enum TITLE_BTUTTON
	{
		TB_GAME,
		TB_MAPTOOL,
		TB_EXIT,
		TB_COUNT
	};
public:
	TitleLevel();
	~TitleLevel();

	TitleLevel(const TitleLevel& _Other) = delete;
	TitleLevel(TitleLevel&& _Other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _Other) = delete;
	TitleLevel& operator=(TitleLevel&& _Other) noexcept = delete;


protected:
	void Loading() override;
	void Update(float _DT) override;
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _NextLevel) override {}
private:
	std::vector<ButtonUI*> vecTitleButtons;
	WiggleGridActor* BackGroundActors = nullptr;

	std::vector<float4> vecCameraMovePoint;
	size_t CameraPointindex = 0;

	WiggleMapToolActor* ButtonBaba = nullptr;
	FadeUI* TitleFadeActor = nullptr;
	int SelectButton = 0;

	void ButtonUp();
	void ButtonDown();
	void ButtonUse();
	void BaBaSetPos();

	void TitleCameraMove(float _DT);
	float GetCameraPointDistance(size_t _Index);
};