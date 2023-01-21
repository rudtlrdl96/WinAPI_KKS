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
		MsgAssert("Nullptr GridActor�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	int2 ActorPos = _Actor->GetGridPos();

	switch (_Actor->GetActorType())
	{
	case ACTOR_TYPE::SUBJECT_TEXT: // ���� ó���̰ų� ���� �Ǵ� ���
		
		break;
	case ACTOR_TYPE::VERB_TEXT: // ���� �߰��� ���
		
		break;
	case ACTOR_TYPE::DEFINE_TEXT:// ���� ���� ���
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
		MsgAssert("�߸��� GridActor Type���� ���� �����Ϸ� �߽��ϴ�.");
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
