#include "RuleManager.h"
#include <GameEngineBase/GameEngineDebug.h>

#include "ContentEnum.h"
#include "GridActor.h"

RuleManager::RuleManager()
{
}

RuleManager::~RuleManager()
{
}


void RuleManager::RuleCheck()
{
	//for (const std::pair<int, GridActor*>& IsActorPair : GridActor::GetActors(TEMP_ACTOR_INDEX::IS_TEXT))
	//{
	//	//GridActor* IsActor = IsActorPair.second;
	//	//GridActor::Rule* CheckTransverseRule = IsActor->CheckTransverseRule();
	//	//
	//	//if (nullptr != CheckTransverseRule)
	//	//{
	//	//	for (:)
	//	//	{
	//	//		switch (CheckTransverseRule->Type)
	//	//		{
	//	//		case RULE_TYPE::IS_SUBJECT:
	//	//
	//	//
	//	//			break;
	//	//		case RULE_TYPE::IS_DEFINE:
	//	//			break;
	//	//		default:
	//	//			break;
	//	//		}
	//	//	}
	//	//}
	//	//
	//	//GridActor::Rule* CheckVerticalRule = IsActor->CheckVerticalRule();
	//	//
	//	//if (nullptr != CheckTransverseRule)
	//	//{
	//	//}
	//}


}

void RuleManager::RuleReset()
{
}