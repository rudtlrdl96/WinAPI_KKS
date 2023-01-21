#include "Rule.h"
#include <GameEngineBase/GameEngineDebug.h>

#include "GridActor.h"

Rule::Rule(int _RuleKey) :
	RuleKey(_RuleKey)
{
}

Rule::~Rule()
{
}

void Rule::CreateRule(GridActor* _Actor)
{
	static int RuleKey = 0;

	if (nullptr == _Actor)
	{
		MsgAssert("Nullptr GridActor를 참조하려 했습니다.");
		return;
	}

	int2 ActorPos = _Actor->GetGridPos();

	switch (_Actor->GetActorType())
	{
	case ACTOR_TYPE::SUBJECT_TEXT: // 내가 처음이거나 끝이 되는 경우
		
		break;
	case ACTOR_TYPE::VERB_TEXT: // 내가 중간인 경우
		
		break;
	case ACTOR_TYPE::DEFINE_TEXT:// 내가 끝인 경우
	{
		{
			int2 VerbPos = ActorPos + int2::Left;
			GridActor* LeftVerbActor = GridActor::GetTextActor(VerbPos);

			if (nullptr != LeftVerbActor && ACTOR_TYPE::VERB_TEXT == LeftVerbActor->GetActorType())
			{

				int2 SubectPos = VerbPos + int2::Left;
				GridActor* LeftSubjectActor = GridActor::GetTextActor(VerbPos);

				if (nullptr != LeftSubjectActor && ACTOR_TYPE::SUBJECT_TEXT == LeftVerbActor->GetActorType())
				{
					return;
				}
			}
		}

		
	}
		break;
	default:
		MsgAssert("잘못된 GridActor Type으로 룰을 생성하려 했습니다.");
		return;
	}
	
	return;
}

void Rule::ActiveRule()
{
}

void Rule::DisableRule()
{
}
