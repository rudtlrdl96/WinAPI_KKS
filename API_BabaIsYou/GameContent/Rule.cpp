#include "Rule.h"
#include <GameEngineBase/GameEngineDebug.h>

#include "GridActor.h"
#include "GridActorManager.h"
#include "RuleManager.h"

Rule::Rule(int _Key) :
	RuleKey(_Key)
{
}

Rule::~Rule()
{
}

void Rule::CreateRule(GridActor* _Actor, bool _IsInit)
{
	static int RuleKey = 0;

	if (nullptr == _Actor)
	{
		MsgAssert("Nullptr GridActor를 참조하려 했습니다.");
		return;
	}

	int2 ActorPos = _Actor->GetGridPos();

	GridActor* HorizontalSubjectActor = nullptr;
	GridActor* HorizontalVerbActor    = nullptr;
	GridActor* HorizontalDefineActor  = nullptr;

	GridActor* BackHorizontalSubjectActor = nullptr;
	GridActor* BackHorizontalVerbActor = nullptr;
	GridActor* BackHorizontalDefineActor = nullptr;

	GridActor* VerticalSubjectActor = nullptr;
	GridActor* VerticalVerbActor    = nullptr;
	GridActor* VerticalDefineActor  = nullptr;

	GridActor* BackVerticalSubjectActor = nullptr;
	GridActor* BackVerticalVerbActor = nullptr;
	GridActor* BackVerticalDefineActor = nullptr;

	switch (_Actor->GetActorType())
	{
	case ACTOR_TYPE::SUBJECT_TEXT: // 내가 처음이거나 끝이 되는 경우
		HorizontalSubjectActor = _Actor;
		HorizontalVerbActor = GridActor::GetTextActor(ActorPos + int2::Right);
		HorizontalDefineActor = GridActor::GetTextActor(ActorPos + int2::Right + int2::Right);
		VerticalSubjectActor = _Actor;
		VerticalVerbActor = GridActor::GetTextActor(ActorPos + int2::Down);
		VerticalDefineActor = GridActor::GetTextActor(ActorPos + int2::Down + int2::Down);

		BackHorizontalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Left + int2::Left);
		BackHorizontalVerbActor = GridActor::GetTextActor(ActorPos + int2::Left);
		BackHorizontalDefineActor = _Actor;
		BackVerticalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Up + int2::Up);
		BackVerticalVerbActor = GridActor::GetTextActor(ActorPos + int2::Up);
		BackVerticalDefineActor = _Actor;
		break;
	case ACTOR_TYPE::VERB_TEXT: // 내가 중간인 경우
		HorizontalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Left);
		HorizontalVerbActor = _Actor;
		HorizontalDefineActor = GridActor::GetTextActor(ActorPos + int2::Right);
		VerticalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Up);
		VerticalVerbActor = _Actor;
		VerticalDefineActor = GridActor::GetTextActor(ActorPos + int2::Down);
		break;
	case ACTOR_TYPE::DEFINE_TEXT:// 내가 끝인 경우
		HorizontalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Left + int2::Left);
		HorizontalVerbActor = GridActor::GetTextActor(ActorPos + int2::Left);
		HorizontalDefineActor = _Actor;
		VerticalSubjectActor = GridActor::GetTextActor(ActorPos + int2::Up + int2::Up);
		VerticalVerbActor = GridActor::GetTextActor(ActorPos + int2::Up);
		VerticalDefineActor = _Actor;
		break;
	default:
		MsgAssert("잘못된 GridActor Type으로 룰을 생성하려 했습니다.");
		return;
	}
	
	if (nullptr != HorizontalSubjectActor && // nullptr check
		nullptr != HorizontalVerbActor && // nullptr check
		nullptr != HorizontalDefineActor && // nullptr check
		ACTOR_TYPE::SUBJECT_TEXT == HorizontalSubjectActor->GetActorType() &&
		ACTOR_TYPE::VERB_TEXT == HorizontalVerbActor->GetActorType() &&
		(ACTOR_TYPE::DEFINE_TEXT == HorizontalDefineActor->GetActorType() || ACTOR_TYPE::SUBJECT_TEXT == HorizontalDefineActor->GetActorType()))
	{
		Rule* CreateRulePtr = new Rule(RuleKey++);	
		RuleManager::GetInst()->AddRule(CreateRulePtr);

		CreateRulePtr->SubjectActor = HorizontalSubjectActor;
		CreateRulePtr->VerbActor = HorizontalVerbActor;
		CreateRulePtr->DefineActor = HorizontalDefineActor;

		const std::map<int, GridActor*> mapSubjectActors = GridActor::GetActors(HorizontalSubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == HorizontalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				if (true == _IsInit)
				{
					Data.second->SetDefine(HorizontalDefineActor->ArrowDefine);
				}
				else
				{
					GridActorManager::GetInst()->AddDefine(Data.second, HorizontalDefineActor->ArrowDefine, false);
				}
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == HorizontalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(HorizontalDefineActor->ArrowEnum, false);
			}
		}

		HorizontalSubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		HorizontalVerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		HorizontalDefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}

	if (nullptr != VerticalSubjectActor && // nullptr check
		nullptr != VerticalVerbActor && // nullptr check
		nullptr != VerticalDefineActor && // nullptr check
		ACTOR_TYPE::SUBJECT_TEXT == VerticalSubjectActor->GetActorType() &&
		ACTOR_TYPE::VERB_TEXT == VerticalVerbActor->GetActorType() &&
		(ACTOR_TYPE::DEFINE_TEXT == VerticalDefineActor->GetActorType() || ACTOR_TYPE::SUBJECT_TEXT == VerticalDefineActor->GetActorType()))
	{
		Rule* CreateRulePtr = new Rule(RuleKey++);
		RuleManager::GetInst()->AddRule(CreateRulePtr);

		CreateRulePtr->SubjectActor = VerticalSubjectActor;
		CreateRulePtr->VerbActor = VerticalVerbActor;
		CreateRulePtr->DefineActor = VerticalDefineActor;

		const std::map<int, GridActor*> mapSubjectActors = GridActor::GetActors(VerticalSubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == VerticalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				GridActorManager::GetInst()->AddDefine(Data.second, VerticalDefineActor->ArrowDefine, false);
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == VerticalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(VerticalDefineActor->ArrowEnum, false);
			}
		}

		VerticalSubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		VerticalVerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		VerticalDefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}

	if (nullptr != BackHorizontalSubjectActor && // nullptr check
		nullptr != BackHorizontalVerbActor && // nullptr check
		nullptr != BackHorizontalDefineActor && // nullptr check
		ACTOR_TYPE::SUBJECT_TEXT == BackHorizontalSubjectActor->GetActorType() &&
		ACTOR_TYPE::VERB_TEXT == BackHorizontalVerbActor->GetActorType() &&
		(ACTOR_TYPE::DEFINE_TEXT == BackHorizontalDefineActor->GetActorType() || ACTOR_TYPE::SUBJECT_TEXT == BackHorizontalDefineActor->GetActorType()))
	{
		Rule* CreateRulePtr = new Rule(RuleKey++);
		RuleManager::GetInst()->AddRule(CreateRulePtr);

		CreateRulePtr->SubjectActor = BackHorizontalSubjectActor;
		CreateRulePtr->VerbActor = BackHorizontalVerbActor;
		CreateRulePtr->DefineActor = BackHorizontalDefineActor;

		const std::map<int, GridActor*> mapSubjectActors = GridActor::GetActors(BackHorizontalSubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == BackHorizontalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				if (true == _IsInit)
				{
					Data.second->SetDefine(BackHorizontalDefineActor->ArrowDefine);
				}
				else
				{
					GridActorManager::GetInst()->AddDefine(Data.second, BackHorizontalDefineActor->ArrowDefine, false);
				}
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == BackHorizontalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(BackHorizontalDefineActor->ArrowEnum, false);
			}
		}

		BackHorizontalSubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		BackHorizontalVerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		BackHorizontalDefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}

	if (nullptr != BackVerticalSubjectActor && // nullptr check
		nullptr != BackVerticalVerbActor && // nullptr check
		nullptr != BackVerticalDefineActor && // nullptr check
		ACTOR_TYPE::SUBJECT_TEXT == BackVerticalSubjectActor->GetActorType() &&
		ACTOR_TYPE::VERB_TEXT == BackVerticalVerbActor->GetActorType() &&
		(ACTOR_TYPE::DEFINE_TEXT == BackVerticalDefineActor->GetActorType() || ACTOR_TYPE::SUBJECT_TEXT == BackVerticalDefineActor->GetActorType()))
	{
		Rule* CreateRulePtr = new Rule(RuleKey++);
		RuleManager::GetInst()->AddRule(CreateRulePtr);

		CreateRulePtr->SubjectActor = BackVerticalSubjectActor;
		CreateRulePtr->VerbActor = BackVerticalVerbActor;
		CreateRulePtr->DefineActor = BackVerticalDefineActor;

		const std::map<int, GridActor*> mapSubjectActors = GridActor::GetActors(BackVerticalSubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == BackVerticalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				GridActorManager::GetInst()->AddDefine(Data.second, BackVerticalDefineActor->ArrowDefine, false);
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == BackVerticalDefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(BackVerticalDefineActor->ArrowEnum, false);
			}
		}

		BackVerticalSubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		BackVerticalVerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		BackVerticalDefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}

	return;
}

void Rule::RemoveRule(GridActor* _Actor)
{
	while (0 < _Actor->mapRules.size())
	{
		Rule* DeleteRule = _Actor->mapRules.begin()->second;
		RuleManager::GetInst()->RemoveRule(DeleteRule);

		if (nullptr == DeleteRule)
		{
			MsgAssert("nullptr Rule을 동적할당 해제하려 했습니다");
		}

		GridActor* SubjectActor = DeleteRule->SubjectActor;
		GridActor* VerbActor = DeleteRule->VerbActor;
		GridActor* DefineActor = DeleteRule->DefineActor;

		if (ACTOR_TYPE::DEFINE_TEXT == DefineActor->ActorType)
		{
			const std::map<int, GridActor*>& mapSubjectActors = GridActor::GetActors(SubjectActor->ArrowEnum);

			for (const std::pair<int, GridActor*> Data : mapSubjectActors)
			{
				GridActorManager::GetInst()->AddDefine(Data.second, DefineActor->ArrowDefine, true);
			}
		}

		std::map<int, Rule*>::iterator SubjectFind = SubjectActor->mapRules.find(DeleteRule->RuleKey);

		if (SubjectActor->mapRules.end() != SubjectFind)
		{
			SubjectActor->mapRules.erase(SubjectFind);
		}

		std::map<int, Rule*>::iterator VerbFind = VerbActor->mapRules.find(DeleteRule->RuleKey);

		if (VerbActor->mapRules.end() != VerbFind)
		{
			VerbActor->mapRules.erase(VerbFind);
		}

		std::map<int, Rule*>::iterator DefineFind = DefineActor->mapRules.find(DeleteRule->RuleKey);

		if (DefineActor->mapRules.end() != DefineFind)
		{
			DefineActor->mapRules.erase(DefineFind);
		}

		delete DeleteRule;
		DeleteRule = nullptr;
	}

	_Actor->mapRules.clear();
}
