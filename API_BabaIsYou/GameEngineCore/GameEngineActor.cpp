#include "GameEngineActor.h"
#include "GameEngineRender.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"

GameEngineActor::GameEngineActor() 
{
}

GameEngineActor::~GameEngineActor() 
{
	for (GameEngineRender* _Render : RenderList)
	{
		if (nullptr == _Render)
		{
			continue;
		}

		delete _Render;
		_Render = nullptr;
	}

	for (GameEngineCollision* _Collision : CollisionList)
	{
		if (nullptr == _Collision)
		{
			continue;
		}

		delete _Collision;
		_Collision = nullptr;
	}

}

GameEngineLevel* GameEngineActor::GetLevel()
{
	return GetOwner<GameEngineLevel>();
}


GameEngineRender* GameEngineActor::CreateRender(const std::string_view& _Image, int _Order /*= 0*/)
{
	GameEngineRender* Render = CreateRender(_Order);
	Render->SetImage(_Image);
	return Render;
}

GameEngineRender* GameEngineActor::CreateRender(int _Order /*= 0*/)
{
	GameEngineRender* Render = new GameEngineRender();
	// �и��� �� ���� ���� �ʴ�.
	Render->SetOwner(this);
	Render->SetOrder(_Order);
	RenderList.push_back(Render);
	return Render;
}

GameEngineCollision* GameEngineActor::CreateCollision(int _GroupIndex)
{
	GameEngineCollision* Collision = new GameEngineCollision();
	// �и��� �� ���� ���� �ʴ�.
	Collision->SetOwner(this);
	Collision->SetOrder(_GroupIndex);
	CollisionList.push_back(Collision);
	return Collision;
}