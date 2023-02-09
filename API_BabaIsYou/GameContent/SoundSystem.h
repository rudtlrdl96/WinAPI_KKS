#pragma once

#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentEnum.h"

#include <GameEnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

class Channel;
class GameEnginePath;
class GameEngineSound;
class SoundSystem : public GameEngineActor
{
public:
	SoundSystem();
	~SoundSystem();

	SoundSystem(const SoundSystem& _Other) = delete;
	SoundSystem(SoundSystem&& _Other) noexcept = delete;
	SoundSystem& operator=(const SoundSystem& _Other) = delete;
	SoundSystem& operator=(SoundSystem&& _Other) noexcept = delete;

	static SoundSystem* GetLevelSoundSystem();


	void SoundLoad(const GameEnginePath& _Path, SOUND_GROUP _SoundGroup);
	void SoundLoad(const std::string_view& _SoundPath, const std::string_view& _SoundName, SOUND_GROUP _SoundGroup);

	void BgmPlay(const std::string_view& _SoundName);
	void BgmStop();

	std::string_view GetBGMName() const
	{
		return CurBGMName;
	}

protected:
	void LevelChangeStart(GameEngineLevel* _PrevLevel) override;
	void LevelChangeEnd(GameEngineLevel* _PrevLevel) override;

private:
	static SoundSystem* LevelSoundSystem;

	static FMOD::Channel* BGMPlayer;
	static std::string CurBGMName;

	std::map<int, std::vector<GameEngineSound*>> mapSoundDatas;
};