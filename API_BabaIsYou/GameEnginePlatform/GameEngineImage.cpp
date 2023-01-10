#include "GameEngineImage.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineWindow.h"

// �ٸ� lib�� ����ϰڴ�.
#pragma comment(lib, "msimg32.lib")

GameEngineImage::GameEngineImage() 
{
}

GameEngineImage::~GameEngineImage() 
{
	if (nullptr != BitMap)
	{
		DeleteObject(BitMap);
		BitMap = nullptr;
	}

	if (nullptr != OldBitMap)
	{
		DeleteObject(OldBitMap);
		OldBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}
}

bool GameEngineImage::ImageCreate(HDC _Hdc)
{
	if (nullptr == _Hdc)
	{
		MsgAssert("�̹��� ������ �����߽��ϴ� nullptr �� HDC�� �־�����ϴ�.");
		return false;
	}

	ImageDC = _Hdc;
	ImageScaleCheck();
	return true;
}

void GameEngineImage::ImageClear()
{
	Rectangle(ImageDC, 0, 0, Info.bmWidth, Info.bmHeight);
}

bool GameEngineImage::ImageCreate(const float4& _Scale)
{
	if (true == _Scale.IsZero())
	{
		MsgAssert("ũ�Ⱑ 0�� �̹����� ���� ���� �����ϴ�");
		return false;
	}

	BitMap = CreateCompatibleBitmap(GameEngineWindow::GetWindowBackBufferHdc(), _Scale.ix(), _Scale.iy());

	if (nullptr == BitMap)
	{
		MsgAssert("�̹��� ������ �����߽��ϴ�.");
		return false;
	}

	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		MsgAssert("�̹��� HDC ������ �����߽��ϴ�.");
		return false;
	}

	// ImageDC 1,1 �迭�̶� ����Ǿ� �ִ�. 

	// 1, 1
	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();

	ImageClear();

	return true;
}

bool GameEngineImage::ImageLoad(const GameEnginePath& _Path)
{
	return ImageLoad(_Path.GetPathToString().c_str());
}

#define TEST(Value) Value

bool GameEngineImage::ImageLoad(const std::string_view& _Path) 
{
	//HDC ImageDC;
	//HBITMAP BitMap;
	//HBITMAP OldBitMap;
	//BITMAP Info;

	// �̹����߿� �Ϻθ� �ε��Ҽ� �ִµ� 0�� �־��ָ� �� �ε��ϰڴٴ� �̾߱Ⱑ �����ϴ�.
	// LR_LOADFROMFILE ���Ͽ������� �ε��ϰڴٴ� �ǹ̰� �˴ϴ�.

	// �̹����� �ε��� 2���� �迭�� ������
	// �����쿡�� new�� �����ѰͰ� �ٸ��� ����.
	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " �̹��� �ε忡 �����߽��ϴ�." );
		return false;
	}

	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " �̹��� HDC ������ �����߽��ϴ�.");
		return false;
	}

	// ImageDC 1,1 �迭�̶� ����Ǿ� �ִ�. 

	// 1, 1
	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();

	return true;
}

void GameEngineImage::ImageScaleCheck()
{
	HBITMAP CurrentBitMap = static_cast<HBITMAP>(GetCurrentObject(ImageDC, OBJ_BITMAP));
	GetObject(CurrentBitMap, sizeof(BITMAP), &Info);
}


// Copy
void GameEngineImage::BitCopy(const GameEngineImage* _OtherImage, float4 _Pos, float4 _Scale)
{
	BitBlt(
		ImageDC, // ���� ���� �̹���
		_Pos.ix() , // ��ġ 
		_Pos.iy() ,
		_Scale.ix(),
		_Scale.iy(),
		_OtherImage->GetImageDC(), // ������ �̹���
		0,
		0,
		SRCCOPY
	);
}

void GameEngineImage::TransCopy(const GameEngineImage* _OtherImage, float4 _CopyPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Color)
{
	// �⺻���� �Լ��� �ƴմϴ�.
	TransparentBlt(ImageDC,
		_CopyPos.ix(),
		_CopyPos.iy(),
		_CopySize.ix(),
		_CopySize.iy(),
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),
		_OtherImagePos.iy(),
		_OtherImageSize.ix(),
		_OtherImageSize.iy(),
		_Color);
}