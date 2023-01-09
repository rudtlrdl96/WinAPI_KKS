#include "GameEngineImage.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineDebug.h>

GameEngineImage::GameEngineImage()
{
}

GameEngineImage::~GameEngineImage()
{
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

bool GameEngineImage::ImageLoad(const GameEnginePath& _Path)
{
	return ImageLoad(_Path.GetPathToString().c_str());
}

bool GameEngineImage::ImageLoad(const std::string_view& _Path)
{
	//HDC ImageDC;
	//HBITMAP BitMap;
	//HBITMAP OldBitMap;
	//BITMAP Info;

	// �̹����߿� �Ϻθ� �ε��Ҽ� �ִµ� 0�� �־��ָ� �� �ε��ϰڴٴ� �̾߱Ⱑ �����ϴ�.
	// LR_LOADFROMFILE ���Ͽ������� �ε��ϰڴٴ� �ǹ̰� �˴ϴ�.

	// �̹����� �ε��� 2���� �迭�� ������
	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " �̹��� �ε忡 �����߽��ϴ�.");
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
	GetObject(BitMap, sizeof(BITMAP), &Info);
}


// Copy
void GameEngineImage::BitCopy(GameEngineImage* _OtherImage, float4 _Pos, float4 _Scale)
{
	BitBlt(
		ImageDC, // ���� ���� �̹���
		_Pos.ix(), // ��ġ 
		_Pos.iy(),
		_Scale.ix(),
		_Scale.iy(),
		_OtherImage->GetImageDC(), // ������ �̹���
		0,
		0,
		SRCCOPY
	);
}