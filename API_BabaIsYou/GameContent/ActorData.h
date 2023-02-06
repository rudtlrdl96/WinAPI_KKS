#pragma once
#include <string>
#include "ContentEnum.h"

class ActorData
{
public:
	ActorData(int _ActorEnum);
	~ActorData();

	const int ActorEnum = -1;
	std::string ActorName = "";
	int RenderIndex = -1;
	int RenderOrder = 0;
	PARTICLE_COLOR Color = PARTICLE_COLOR::WHITE;
	
	ACTOR_TYPE ActorType = ACTOR_TYPE::ACTOR;
	ACTOR_RENDER_TYPE RenderType = ACTOR_RENDER_TYPE::STATIC;

	int ArrowEnum = -1;
	ACTOR_DEFINE ArrowDefine = ACTOR_DEFINE::NONE;

protected:

private:

};