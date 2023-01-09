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
		MsgAssert("이미지 생성에 실패했습니다 nullptr 인 HDC를 넣어줬습니다.");
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

	// 이미지중에 일부만 로드할수 있는데 0을 넣어주면 다 로드하겠다는 이야기가 도힙니다.
	// LR_LOADFROMFILE 파일에서부터 로드하겠다는 의미가 됩니다.

	// 이미지를 로드한 2차원 배열의 정보고
	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 로드에 실패했습니다.");
		return false;
	}

	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 HDC 생성에 실패했습니다.");
		return false;
	}

	// ImageDC 1,1 배열이랑 연결되어 있다. 

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
		ImageDC, // 복사 당할 이미지
		_Pos.ix(), // 위치 
		_Pos.iy(),
		_Scale.ix(),
		_Scale.iy(),
		_OtherImage->GetImageDC(), // 복사할 이미지
		0,
		0,
		SRCCOPY
	);
}