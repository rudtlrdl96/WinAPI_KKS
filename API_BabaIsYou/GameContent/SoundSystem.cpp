#include "SoundSystem.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineResources.h>
#include <GameEnginePlatform/GameEngineSound.h>
#include "ContentRand.h"

SoundSystem* SoundSystem::LevelSoundSystem = nullptr;
FMOD::Channel* SoundSystem::BGMPlayer = nullptr;
std::string SoundSystem::CurBGMName;

std::map<std::string, GameEngineSound*> SoundSystem::mapAllSoundDatas;
std::map<int, std::vector<GameEngineSound*>> SoundSystem::mapGroupSoundDatas;
std::map<std::string, std::vector<GameEngineSound*>> SoundSystem::mapEffectSoundDatas;


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
		MsgAssert("사운드 시스템이 중복 생성되었습니다.");
		return;
	}

	LevelSoundSystem = this;
}

void SoundSystem::LevelChangeEnd(GameEngineLevel* _PrevLevel)
{
	if (nullptr == LevelSoundSystem)
	{
		MsgAssert("사운드 시스템이 중복 생성되었습니다.");
		return;
	}

	LevelSoundSystem = nullptr;
}

SoundSystem* SoundSystem::GetLevelSoundSystem()
{
	if (nullptr == LevelSoundSystem)
	{
		MsgAssert("사운드 시스템을 생성하지 않고 사용하려 했습니다.");
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
	if (mapAllSoundDatas.find(_SoundName.data()) != mapAllSoundDatas.end())
	{
		MsgAssert("사운드를 중복 생성했습니다");
		return;
	}

	GameEngineSound* Sound = GameEngineResources::GetInst().SoundLoad(_SoundPath, _SoundName);
	mapGroupSoundDatas[static_cast<int>(_SoundGroup)].push_back(Sound);
	mapAllSoundDatas.insert({ _SoundName.data(), Sound });
}

void SoundSystem::EffectSoundLoad(const GameEnginePath& _Path, const std::string_view& _Extensions, SOUND_GROUP _SoundGroup, int _Count)
{
	EffectSoundLoad(_Path.GetPathToString().c_str(), _Path.GetFileName().c_str(), _Extensions, _SoundGroup, _Count);
}

void SoundSystem::EffectSoundLoad(const std::string_view& _SoundPath, const std::string_view& _SoundName, const std::string_view& _Extensions, SOUND_GROUP _SoundGroup, int _Count)
{
	if (mapAllSoundDatas.find(_SoundName.data()) != mapAllSoundDatas.end())
	{
		MsgAssert("사운드를 중복 생성했습니다");
		return;
	}

	mapEffectSoundDatas[_SoundName.data()].reserve(_Count);

	for (size_t i = 0; i < _Count; i++)
	{
		std::string SoundName = _SoundName.data() + std::to_string(i) + _Extensions.data();
		GameEngineSound* Sound = GameEngineResources::GetInst().SoundLoad(_SoundPath.data() + std::to_string(i) + _Extensions.data(), SoundName);
		mapGroupSoundDatas[static_cast<int>(_SoundGroup)].push_back(Sound);

		mapEffectSoundDatas[_SoundName.data()].push_back(Sound);
		mapAllSoundDatas.insert({ SoundName.data(), Sound });
	}
}

void SoundSystem::Play(const std::string_view& _SoundName, bool _IsLoop)
{
	FMOD::Channel* PlaySoundChannel = GameEngineResources::GetInst().SoundFind(_SoundName)->Play();

	if (true == _IsLoop)
	{
		PlaySoundChannel->setLoopCount(INT32_MAX);
	}
	else
	{
		PlaySoundChannel->setLoopCount(0);
	}
}

void SoundSystem::BgmPlay(const std::string_view& _SoundName, bool _IsPause)
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
	BGMPlayer->removeFadePoints(0, INT64_MAX);
	BGMPlayer->addFadePoint(ParentClock, 0.0f);
	BGMPlayer->addFadePoint(static_cast<unsigned long long>(ParentClock + (Rate * 1)), 1.0f);
}

void SoundSystem::BgmStop(bool _IsPause)
{
	if (nullptr == BGMPlayer)
	{
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;
	float CurVolume = 0.0f;

	FMOD::System* SoundSys = nullptr;
	BGMPlayer->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	BGMPlayer->getDSPClock(nullptr, &ParentClock);
	BGMPlayer->getVolume(&CurVolume);

	BGMPlayer->removeFadePoints(0, INT64_MAX);
	BGMPlayer->addFadePoint(ParentClock, CurVolume);
	BGMPlayer->addFadePoint(static_cast<unsigned long long>(ParentClock + (Rate * 1)), static_cast<unsigned __int64>(0.0f));
	BGMPlayer->setDelay(static_cast<unsigned long long>(ParentClock + (Rate * 1)), static_cast<unsigned __int64>(0.0f));

	CurBGMName = "";
	BGMPlayer = nullptr;
}

void SoundSystem::EffectPlay(const std::string_view& _EffectSoundName)
{
	std::map<std::string, std::vector<GameEngineSound*>>::iterator FindIter = mapEffectSoundDatas.find(_EffectSoundName.data());

	if (FindIter == mapEffectSoundDatas.end())
	{
		MsgAssert("이펙트 사운드를 초기화 하지 않고 사용하려 했습니다");
		return;
	}

	int RandIndex = ContentRand::RandInt(0, static_cast<int>((*FindIter).second.size() - 1));
	(*FindIter).second[RandIndex]->Play()->setLoopCount(0);
}