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

	void EffectSoundLoad(const GameEnginePath& _Path, const std::string_view& _Extensions, SOUND_GROUP _SoundGroup, int _Count);
	void EffectSoundLoad(const std::string_view& _SoundPath, const std::string_view& _SoundName, const std::string_view& _Extensions, SOUND_GROUP _SoundGroup, int _Count);

	void Play(const std::string_view& _SoundName, bool _IsLoop = false);

	void BgmPlay(const std::string_view& _SoundName, bool _IsPause = false);
	void BgmStop(bool _IsPause = false);
	
	void EffectPlay(const std::string_view& _EffectSoundName);

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

	static std::map<std::string, GameEngineSound*> mapAllSoundDatas;
	static std::map<int, std::vector<GameEngineSound*>> mapGroupSoundDatas;
	static std::map<std::string, std::vector<GameEngineSound*>> mapEffectSoundDatas;
};