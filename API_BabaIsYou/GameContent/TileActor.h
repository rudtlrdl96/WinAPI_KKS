#pragma once
#include "SubjectActor.h"
#include <map>

class TileActor : public SubjectActor
{
private:
	enum TILE_NEIGHBOR
	{
		NONE	= 0,
		LEFT	= 1 << 0,
		RIGHT	= 1 << 1,
		UP		= 1 << 2,
		DOWN	= 1 << 3,
	};

public:
	TileActor();
	~TileActor();

	TileActor(const TileActor& _Other) = delete;
	TileActor(TileActor&& _Other) noexcept = delete;
	TileActor& operator=(const TileActor& _Other) = delete;
	TileActor& operator=(TileActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	
	// 모튼 타일 클래스는 반드시 방향에따른 비트맵 데이터 필수 생성
	virtual void SetBitmap() = 0;

private:
	TILE_NEIGHBOR Neighbor = TILE_NEIGHBOR::NONE;

	// Todo : Bitmap Render 클래스가 추가될 경우 int -> Bitmap 출력 클래스로 변경
	std::map<TILE_NEIGHBOR, int> mapTileBitmap;
};