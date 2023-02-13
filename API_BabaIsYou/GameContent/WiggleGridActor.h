#pragma once
#include <vector>
#include <GameEngineCore/GameEngineActor.h>
#include "ContentMath.h"
#include "ContentEnum.h"

class WiggleMapToolActor;
class WiggleGridActor : public GameEngineActor
{
public:
	WiggleGridActor();
	~WiggleGridActor();

	WiggleGridActor(const WiggleGridActor& _Other) = delete;
	WiggleGridActor(WiggleGridActor&& _Other) noexcept = delete;
	WiggleGridActor& operator=(const WiggleGridActor& _Other) = delete;
	WiggleGridActor& operator=(WiggleGridActor&& _Other) noexcept = delete;

	void InitGrid(const int2& _Size);
	void SetRender(const int2& _Pos, int _ActorEnum, DIR_FLAG _Dir);
	bool IsOver(const int2& _Pos) const;

	int GetActorEnum(const int2& _Pos) const;
	int GetDir(const int2& _Pos) const;

	void ActiveGridActors();
	void DisableGridActors();
	void ResetGridActors();
	void TileCheck(const int2& _Pos);

	inline void SetGridSize(int2 _GridSize) 
	{
		GridSize = _GridSize;
	}

	inline int2 GetGridSize() const
	{
		return GridSize;
	}

protected:

private:
	int2 GridSize;
	std::vector<std::vector<WiggleMapToolActor*>> vecWiggleActors;

	int GetNeighborFlagKey(const int2& _Pos);
	bool IsEqulsEnum(const int2& _Pos1, const int2& _Pos2);
};