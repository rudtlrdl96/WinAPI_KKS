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
	
	// ��ư Ÿ�� Ŭ������ �ݵ�� ���⿡���� ��Ʈ�� ������ �ʼ� ����
	virtual void SetBitmap() = 0;

private:
	TILE_NEIGHBOR Neighbor = TILE_NEIGHBOR::NONE;

	// Todo : Bitmap Render Ŭ������ �߰��� ��� int -> Bitmap ��� Ŭ������ ����
	std::map<TILE_NEIGHBOR, int> mapTileBitmap;
};