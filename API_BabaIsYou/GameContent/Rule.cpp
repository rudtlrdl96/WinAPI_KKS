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
	if (nullptr == _Actor)
	{
		MsgAssert("Nullptr GridActor를 참조하려 했습니다.");
		return;
	}

	int2 ActorPos = _Actor->GetGridPos();

	switch (_Actor->GetActorType())
	{
	case ACTOR_TYPE::SUBJECT_TEXT: // 내가 처음이거나 끝이 되는 경우
		CheckRule(
			_Actor,
			GridActor::GetTextActor(ActorPos + int2::Right), 
			GridActor::GetTextActor(ActorPos + int2::Right + int2::Right),
			_IsInit);

		CheckRule(
			_Actor,
			GridActor::GetTextActor(ActorPos + int2::Down),
			GridActor::GetTextActor(ActorPos + int2::Down + int2::Down),
			_IsInit);

		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Left + int2::Left),
			GridActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			_IsInit);

		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Up + int2::Up),
			GridActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			_IsInit);
		break;
	case ACTOR_TYPE::VERB_TEXT: // 내가 중간인 경우
		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			GridActor::GetTextActor(ActorPos + int2::Right),
			_IsInit);

		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			GridActor::GetTextActor(ActorPos + int2::Down),
			_IsInit);
		break;
	case ACTOR_TYPE::DEFINE_TEXT:// 내가 끝인 경우

		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Left + int2::Left),
			GridActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			_IsInit);

		CheckRule(
			GridActor::GetTextActor(ActorPos + int2::Up + int2::Up),
			GridActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			_IsInit);
		break;
	default:
		MsgAssert("잘못된 GridActor Type으로 룰을 생성하려 했습니다.");
		return;
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

void Rule::CheckRule(GridActor* _SubjectActor, GridActor* _VerbActor, GridActor* _DefineActor, bool _IsInit)
{
	static int RuleKey = 0;

	if (nullptr != _SubjectActor && // nullptr check
		nullptr != _VerbActor && // nullptr check
		nullptr != _DefineActor && // nullptr check
		ACTOR_TYPE::SUBJECT_TEXT == _SubjectActor->GetActorType() &&
		ACTOR_TYPE::VERB_TEXT == _VerbActor->GetActorType() &&
		(ACTOR_TYPE::DEFINE_TEXT == _DefineActor->GetActorType() || ACTOR_TYPE::SUBJECT_TEXT == _DefineActor->GetActorType()))
	{
		Rule* CreateRulePtr = new Rule(RuleKey++);
		RuleManager::GetInst()->AddRule(CreateRulePtr);

		CreateRulePtr->SubjectActor = _SubjectActor;
		CreateRulePtr->VerbActor = _VerbActor;
		CreateRulePtr->DefineActor = _DefineActor;

		const std::map<int, GridActor*> mapSubjectActors = GridActor::GetActors(_SubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == _DefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				if (true == _IsInit)
				{
					Data.second->SetDefine(_DefineActor->ArrowDefine);
				}
				else
				{
					GridActorManager::GetInst()->AddDefine(Data.second, _DefineActor->ArrowDefine, false);
				}
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == _DefineActor->GetActorType())
		{
			for (const std::pair<int, GridActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(_DefineActor->ArrowEnum, false);
			}
		}

		_SubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		_VerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		_DefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}
}
