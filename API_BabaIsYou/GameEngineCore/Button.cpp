#include "Button.h"
#include <GameEnginePlatform/GameEngineInput.h>

Button::Button() 
{
}

Button::~Button() 
{
}

void Button::SetImage() 
{

}

void Button::SetTargetCollisionGroup(int _PointTargetGroup)
{
	PointTargetGroup = _PointTargetGroup;
	if (nullptr == ButtonCollision)
	{
		ButtonCollision = CreateCollision(PointTargetGroup);
	}
}


void Button::Start()
{
	
	Render = CreateRender();
}

void Button::SetScale() 
{
	//int TargetGroup = -342367842;
	//CollisionType TargetColType = CollisionType::CT_CirCle;
	//CollisionType ThisColType = CollisionType::CT_CirCle;

	if (true == ButtonCollision->Collision({ .TargetGroup = PointTargetGroup, .TargetColType = CollisionType::CT_Point, .ThisColType = ButtonCollisionType }))
	{
		if (true == GameEngineInput::IsDown("EngineMouseLeft") && nullptr != ClickPtr)
		{
			ClickPtr();
		}
	}
}