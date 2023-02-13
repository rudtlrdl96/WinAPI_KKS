#include "ButtonUI.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include <GameEngineCore/GameEngineRender.h>

#include "ContentEnum.h"
#include "SoundSystem.h"

ButtonUI::ButtonUI()
{
}

ButtonUI::~ButtonUI()
{
}

void ButtonUI::SetImage(const std::string_view& _ImageName, float4 _Scale)
{
	if (nullptr != ImageRender)
	{
		MsgAssert("버튼 이미지를 중복 생성하려 합니다");
		return;
	}

	SetColSize(_Scale);
	ImageRender = CreateRender(_ImageName, RENDER_ORDER::BUTTON);
	ImageRender->SetScale(_Scale);
	ImageRender->EffectCameraOff();
}

void ButtonUI::Start()
{
}

void ButtonUI::Update(float _DT)
{
	float4 CenterPos = GetPos() + ColOffset;
	float4 HalfSize = ColSize.half();
	POINT MousePos;

	if (false == GetCursorPos(&MousePos))
	{
		return;
	}

	ScreenToClient(GameEngineWindow::GetHWnd(), &MousePos);

	// AABB
	if (MousePos.x > CenterPos.x - HalfSize.ix() &&
		MousePos.x < CenterPos.x + HalfSize.ix() &&
		MousePos.y > CenterPos.y - HalfSize.iy() &&
		MousePos.y < CenterPos.y + HalfSize.iy())
	{
		if (false == IsOverlapValue)
		{
			IsOverlapValue = true;
			SoundSystem::GetLevelSoundSystem()->EffectPlay("ButtonOverlap_");
		}
	
		if (nullptr != ImageRender)
		{
			if (true == IsSelectValue)
			{
				ImageRender->SetFrame(3);
			}
			else
			{
				ImageRender->SetFrame(1);
			}
		}

		if (true == GameEngineInput::IsPress("MouseLeft"))
		{
			IsClickValue = true;
		}
		else
		{
			IsClickValue = false;
		}

		if (true == GameEngineInput::IsUp("MouseLeft"))
		{
			if (IsUpValue == false)
			{
				SoundSystem::GetLevelSoundSystem()->Play("ButtonClick.ogg");
				IsUpValue = true;
			}
		}
		else
		{
			IsUpValue = false;
		}
	}
	else
	{
		if (nullptr != ImageRender)
		{
			if (true == IsSelectValue)
			{
				ImageRender->SetFrame(2);
			}
			else
			{
				ImageRender->SetFrame(0);
			}
		}

		IsOverlapValue = false;
		IsClickValue = false;
		IsUpValue = false;
	}
}

void ButtonUI::Render(float _DT)
{
#ifdef _DEBUG
	HDC DoubleDC = GameEngineWindow::GetDoubleBufferImage()->GetImageDC();
	float4 ActorPos = GetPos() + ColOffset;

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(DoubleDC, myBrush);

	HPEN hNewPen = nullptr;

	if (true == IsClickValue)
	{
		hNewPen = CreatePen(PS_DOT, 2, RGB(255, 0, 0));
	}
	else if (true == IsOverlapValue)
	{
		hNewPen = CreatePen(PS_DOT, 2, RGB(0, 0, 255));
	}
	else
	{
		hNewPen = CreatePen(PS_DOT, 2, RGB(0, 255, 0));
	}

	HPEN hPrevPen = (HPEN)SelectObject(DoubleDC, hNewPen);

	Rectangle(DoubleDC,
		ActorPos.ix() - ColSize.half().ix(),
		ActorPos.iy() - ColSize.half().iy(),
		ActorPos.ix() + ColSize.half().ix(),
		ActorPos.iy() + ColSize.half().iy()
	);

	SelectObject(DoubleDC, hPrevPen); //현재영역에 대한 펜을 원래 있던 펜으로 다시 적용
	DeleteObject(hNewPen); //사용이 끝난 펜 해제

	SelectObject(DoubleDC, oldBrush);
	DeleteObject(myBrush);

#endif
}