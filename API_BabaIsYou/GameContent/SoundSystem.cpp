#include "SoundSystem.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEnginePlatform/GameEngineSound.h>

SoundSystem* SoundSystem::LevelSoundSystem = nullptr;
FMOD::Channel* SoundSystem::BGMPlayer = nullptr;
std::string SoundSystem::CurBGMName;

SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	if (nullptr != LevelSoundSystem)
	{
		MsgAssert("���� �ý����� �ߺ� �����Ǿ����ϴ�.");
		return;
	}

	LevelSoundSystem = this;
}

void SoundSystem::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	if (nullptr == LevelSoundSystem)
	{
		MsgAssert("���� �ý����� �ߺ� �����Ǿ����ϴ�.");
		return;
	}

	LevelSoundSystem = nullptr;
}

SoundSystem* SoundSystem::GetLevelSoundSystem()
{
	if (nullptr == LevelSoundSystem)
	{
		MsgAssert("���� �ý����� �������� �ʰ� ����Ϸ� �߽��ϴ�.");
		return nullptr;
	}

	return LevelSoundSystem;
}

void SoundSystem::SoundLoad(const GameEnginePath& _Path, SOUND_GROUP _SoundGroup)
{
	SoundLoad(_Path.GetPathToString().c_str(), _Path.GetFileName().c_str(), _SoundGroup);
}

void SoundSystem::SoundLoad(const std::string_view& _SoundPath, const std::string_view& _SoundName, SOUND_GROUP _SoundGroup)
{
	GameEngineSound* Sound = GameEngineResources::GetInst().SoundLoad(_SoundPath, _SoundName);
	mapSoundDatas[static_cast<int>(_SoundGroup)].push_back(Sound);
}

void SoundSystem::BgmPlay(const std::string_view& _SoundName)
{
	if (CurBGMName == _SoundName)
	{
		return;
	}

	if (nullptr != BGMPlayer)
	{
		BGMPlayer->stop();
	}

	CurBGMName = _SoundName;
	BGMPlayer = GameEngineResources::GetInst().SoundFind(CurBGMName)->Play();
	BGMPlayer->setLoopCount(INT32_MAX);

	int Rate = 0;
	unsigned long long ParentClock = 0u;

	FMOD::System* SoundSys = nullptr;
	BGMPlayer->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	BGMPlayer->getDSPClock(nullptr, &ParentClock);
	BGMPlayer->removeFadePoints(ParentClock, static_cast<unsigned long long>(ParentClock + (Rate * 1.2)));
	BGMPlayer->addFadePoint(ParentClock, 0.0f);
	BGMPlayer->addFadePoint(static_cast<unsigned long long>(ParentClock + (Rate * 1.2)), 1.0f);
}

void SoundSystem::BgmStop()
{
	if (nullptr == BGMPlayer)
	{
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;

	FMOD::System* SoundSys = nullptr;
	BGMPlayer->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	BGMPlayer->getDSPClock(nullptr, &ParentClock);
	BGMPlayer->removeFadePoints(ParentClock, static_cast<unsigned long long>(ParentClock + (Rate * 0.6)));
	BGMPlayer->addFadePoint(ParentClock, 1.0f);
	BGMPlayer->addFadePoint(static_cast<unsigned long long>(ParentClock + (Rate * 0.6)), 0.0f);
	BGMPlayer->setDelay(static_cast<unsigned long long>(ParentClock + (Rate * 0.6)), true);
	CurBGMName = "";
	BGMPlayer = nullptr;
}