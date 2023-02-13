#include "LogoLevel.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineResources.h>
#include "LogoUI.h"
#include "FadeUI.h"
#include "BlackBackUI.h"
#include "ContentFunc.h"
#include "ContentDataBase.h"
#include "SoundSystem.h"
#include "ContentEnum.h"

LogoLevel::LogoLevel()
{
}

LogoLevel::~LogoLevel()
{
}

void LogoLevel::Loading()
{
	GameEngineDirectory Dir;

	Dir.MoveParentToDirectory("ContentsResources");
	Dir.Move("ContentsResources");
	Dir.Move("Bitmap");
	Dir.Move("Logo");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Logo.BMP"))->Cut(1, 3);

	Dir.MoveParent();
	Dir.Move("Defalut");

	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("actor.BMP"))->Cut(24, 40);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Particle.BMP"))->Cut(7, 44);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Gray.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Background_Black.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeCircle.BMP"));
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("FadeAnim.BMP"))->Cut(1, 35);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("Text.BMP"))->Cut(38, 2);
	GameEngineResources::GetInst().ImageLoad(Dir.GetPlusFileName("WiggleText.BMP"))->Cut(37, 6);


	GameEngineDirectory SoundDir;

	SoundDir.MoveParentToDirectory("ContentsResources");
	SoundDir.Move("ContentsResources");
	SoundDir.Move("Sound");
	SoundDir.Move("BGM");

	SoundSystemPtr = CreateActor<SoundSystem>();
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("menu.ogg"), SOUND_GROUP::BGM);

	SoundDir.MoveParent();
	SoundDir.Move("Effect");	

	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("TitleOut.ogg"), SOUND_GROUP::EFFECT);

	SoundDir.MoveParent();
	SoundDir.Move("UI");
	SoundSystemPtr->EffectSoundLoad(SoundDir.GetPlusFileName("ButtonOverlap_"), ".ogg", SOUND_GROUP::EFFECT, 5);
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("ButtonClick.ogg"), SOUND_GROUP::EFFECT);
	SoundSystemPtr->SoundLoad(SoundDir.GetPlusFileName("MapToolPen.ogg"), SOUND_GROUP::EFFECT);

	ContentDataBase::GetInst()->InitDataBase();

	LogoFadeActor = CreateActor<FadeUI>(1);
	CreateActor<LogoUI>(0);
	CreateActor<BlackBackUI>(-1);
}

void LogoLevel::Update(float _DT)
{
	if (false == LogoFadeActor->IsProgress() && true == GameEngineInput::IsAnyKey())
	{
		LogoFadeActor->Fade({ .State = FADE_STATE::FADEIN, .Func = ContentFunc::ChangeTitleLevel});
	}
}

void LogoLevel::LevelChangeStart(GameEngineLevel* _NextLevel)
{
	LogoFadeActor->Fade({ .State = FADE_STATE::FADEOUT});
	SoundSystemPtr->BgmPlay("Menu.ogg");
}
