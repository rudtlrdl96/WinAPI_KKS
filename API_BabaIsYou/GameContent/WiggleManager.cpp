#include "WiggleManager.h"
#include "ContentConst.h"
#include "WiggleActor.h"

float WiggleManager::PrograssTime = 0.0f;

WiggleManager::WiggleManager()
{
}

WiggleManager::~WiggleManager()
{
}

void WiggleManager::Update()
{
	// Todo : Time �ý����� ������ ��� deltaTime���� ����
	PrograssTime += 0.01f;

	if (PrograssTime >= ContentConst::WIGGLE_TIME)
	{
		PrograssTime = 0.0f;
		WiggleActor::Wiggle();
	}
}
