#pragma once
#include "GameEngineActor.h"
#include "GameEngineCollision.h"
#include "GameEngineRender.h"

// Ό³Έν :
class GameEngineRender;
class Button : public GameEngineActor
{
public:
	// constrcuter destructer
	Button();
	~Button();

	// delete Function
	Button(const Button& _Other) = delete;
	Button(Button&& _Other) noexcept = delete;
	Button& operator=(const Button& _Other) = delete;
	Button& operator=(Button&& _Other) noexcept = delete;

	void SetImage();
	void SetScale();

	void SetClickCallBack(void(*_ClickPtr)())
	{
		ClickPtr = _ClickPtr;
	}

	void SetTargetCollisionGroup(int _PointTargetGroup);

	void SetButtonCollisionType(CollisionType _ButtonCollisionType)
	{
		ButtonCollisionType = _ButtonCollisionType;
	}

protected:
	void Start() override;

private:
	GameEngineRender* Render = nullptr;
	GameEngineCollision* ButtonCollision = nullptr;
	int PointTargetGroup = 0;
	CollisionType ButtonCollisionType = CollisionType::CT_Rect;
	void(*ClickPtr)() = nullptr;

};

