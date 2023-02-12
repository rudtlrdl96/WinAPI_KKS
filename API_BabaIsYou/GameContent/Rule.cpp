#include "Rule.h"
#include <GameEngineBase/GameEngineDebug.h>

#include "PuzzleActor.h"
#include "PuzzleActorManager.h"
#include "RuleManager.h"
#include "SoundSystem.h"

Rule::Rule(int _Key) :
	RuleKey(_Key)
{
}

Rule::~Rule()
{
}

void Rule::CreateRule(PuzzleActor* _Actor, bool _IsInit)
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
			PuzzleActor::GetTextActor(ActorPos + int2::Right),
			PuzzleActor::GetTextActor(ActorPos + int2::Right + int2::Right),
			_IsInit);

		CheckRule(
			_Actor,
			PuzzleActor::GetTextActor(ActorPos + int2::Down),
			PuzzleActor::GetTextActor(ActorPos + int2::Down + int2::Down),
			_IsInit);

		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Left + int2::Left),
			PuzzleActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			_IsInit);

		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Up + int2::Up),
			PuzzleActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			_IsInit);
		break;
	case ACTOR_TYPE::VERB_TEXT: // 내가 중간인 경우
		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			PuzzleActor::GetTextActor(ActorPos + int2::Right),
			_IsInit);

		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			PuzzleActor::GetTextActor(ActorPos + int2::Down),
			_IsInit);
		break;
	case ACTOR_TYPE::DEFINE_TEXT:// 내가 끝인 경우

		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Left + int2::Left),
			PuzzleActor::GetTextActor(ActorPos + int2::Left),
			_Actor,
			_IsInit);

		CheckRule(
			PuzzleActor::GetTextActor(ActorPos + int2::Up + int2::Up),
			PuzzleActor::GetTextActor(ActorPos + int2::Up),
			_Actor,
			_IsInit);
		break;
	default:
		MsgAssert("잘못된 GridActor Type으로 룰을 생성하려 했습니다.");
		return;
	}
	
	return;
}

void Rule::RemoveRule(PuzzleActor* _Actor)
{
	while (0 < _Actor->mapRules.size())
	{
		Rule* DeleteRule = _Actor->mapRules.begin()->second;
		RuleManager::GetInst()->RemoveRule(DeleteRule);

		if (nullptr == DeleteRule)
		{
			MsgAssert("nullptr Rule을 동적할당 해제하려 했습니다");
		}

		PuzzleActor* SubjectActor = DeleteRule->SubjectActor;
		PuzzleActor* VerbActor = DeleteRule->VerbActor;
		PuzzleActor* DefineActor = DeleteRule->DefineActor;

		if (ACTOR_TYPE::DEFINE_TEXT == DefineActor->ActorType)
		{
			const std::map<int, PuzzleActor*>& mapSubjectActors = PuzzleActor::GetActors(SubjectActor->ArrowEnum);

			for (const std::pair<int, PuzzleActor*> Data : mapSubjectActors)
			{
				PuzzleActorManager::GetInst()->AddDefine(Data.second, DefineActor->ArrowDefine, true);
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

void Rule::CheckRule(PuzzleActor* _SubjectActor, PuzzleActor* _VerbActor, PuzzleActor* _DefineActor, bool _IsInit)
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

		const std::map<int, PuzzleActor*> mapSubjectActors = PuzzleActor::GetActors(_SubjectActor->ArrowEnum);

		if (ACTOR_TYPE::DEFINE_TEXT == _DefineActor->GetActorType())
		{
			for (const std::pair<int, PuzzleActor*>& Data : mapSubjectActors)
			{
				if (true == _IsInit)
				{
					Data.second->SetDefine(_DefineActor->ArrowDefine);
				}
				else
				{
					PuzzleActorManager::GetInst()->AddDefine(Data.second, _DefineActor->ArrowDefine, false);
				}
			}

			if (false == _IsInit)
			{
				SoundSystem::GetLevelSoundSystem()->EffectPlay("TextCompletion_");
			}
		}

		if (ACTOR_TYPE::SUBJECT_TEXT == _DefineActor->GetActorType())
		{
			for (const std::pair<int, PuzzleActor*>& Data : mapSubjectActors)
			{
				Data.second->LoadData(_DefineActor->ArrowEnum, Data.second->GetMoveDir(), false);
			}
			SoundSystem::GetLevelSoundSystem()->EffectPlay("TextCompletion_");
		}

		_SubjectActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		_VerbActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
		_DefineActor->mapRules.insert({ CreateRulePtr->RuleKey, CreateRulePtr });
	}
}
