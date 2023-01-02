#pragma once


class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		BABA		,
		FLAG		,
		WALL		,
		ROCK		,
		WATER		,
		SKULL		,
		LAVA		,
		GRASS		,
		BLOCK		,
		FLOORTILE	,
		FLOWER		,
	};

public:
	GameObject();
	~GameObject();

	GameObject(const GameObject& _Other) = delete;
	GameObject(GameObject&& _Other) noexcept = delete;
	GameObject& operator=(const GameObject& _Other) = delete;
	GameObject& operator=(GameObject&& _Other) noexcept = delete;

protected:

private:
	GAMEOBJECT_TYPE ObjectType = GAMEOBJECT_TYPE::BABA;
};