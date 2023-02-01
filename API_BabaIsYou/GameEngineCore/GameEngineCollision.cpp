#include "GameEngineCollision.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

static bool(*ColFunctionPtr[CT_Max][CT_Max])(const CollisionData& _Left, const CollisionData& _Right);


class CollisionFunctionInit
{
public:
	CollisionFunctionInit() 
	{
		ColFunctionPtr[CT_CirCle][CT_CirCle] = GameEngineCollision::CollisionCirCleToCirCle;
	}
	~CollisionFunctionInit() 
	{

	}
};

CollisionFunctionInit Init = CollisionFunctionInit();

GameEngineCollision::GameEngineCollision() 
{
}

GameEngineCollision::~GameEngineCollision() 
{
}

bool GameEngineCollision::CollisionCirCleToCirCle(const CollisionData& _Left, const CollisionData& _Right)
{
	float4 Len = _Left.Position - _Right.Position;
	float Size = Len.Size();
	float RadiusSum = _Left.Scale.hx() + _Right.Scale.hx();
	return RadiusSum > Size;
}

void GameEngineCollision::SetOrder(int _Order)
{
	GameEngineComponent::SetOrder(_Order);
	GetActor()->GetLevel()->PushCollision(this);
}

bool GameEngineCollision::Collision(const CollisionCheckParameter& _Parameter)
{
	std::list<GameEngineCollision*>& _TargetGroup = GetActor()->GetLevel()->Collisions[_Parameter.TargetGroup];

	for (GameEngineCollision* OtherCollision : _TargetGroup)
	{
		CollisionType Type = _Parameter.ThisColType;
		CollisionType OtherType = _Parameter.TargetColType;

		if (nullptr == ColFunctionPtr[Type][OtherType])
		{
			MsgAssert("아직 충돌함수를 만들지 않은 충돌 타입입니다");
		}

		if (true == ColFunctionPtr[Type][OtherType](GetCollisionData(), OtherCollision->GetCollisionData()))
		{
			return true;
		}
	}
	return false;
}

bool GameEngineCollision::Collision(const CollisionCheckParameter& _Parameter, std::vector<GameEngineCollision*>& _Collision)
{
	_Collision.clear();

	std::list<GameEngineCollision*>& _TargetGroup = GetActor()->GetLevel()->Collisions[_Parameter.TargetGroup];

	for (GameEngineCollision* OtherCollision : _TargetGroup)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		CollisionType Type = _Parameter.ThisColType;
		CollisionType OtherType = _Parameter.TargetColType;

		if (nullptr == ColFunctionPtr[Type][OtherType])
		{
			MsgAssert("아직 충돌함수를 만들지 않은 충돌 타입입니다");
		}

		if (true == ColFunctionPtr[Type][OtherType](GetCollisionData(), OtherCollision->GetCollisionData()))
		{
			_Collision.push_back(OtherCollision);
		}
	}

	return _Collision.size() != 0;
}

CollisionData GameEngineCollision::GetCollisionData()
{
	return { GetActorPlusPos(), GetScale()};
}