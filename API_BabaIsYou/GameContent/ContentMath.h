#pragma once
#include <GameEngineBase/GameEngineMath.h>

class int2
{
public:
	static const int2 Zero;
	static const int2 Left;
	static const int2 Right;
	static const int2 Up;
	static const int2 Down;

public:
	int x = 0;
	int y = 0;

	int2() :
		x(0),
		y(0)
	{
	}

	int2(int _x, int _y) :
		x(_x),
		y(_y)
	{
	}

	int2& operator=(const int2& _Other)
	{
		this->x = _Other.x;
		this->y = _Other.y;

		return *this;
	}

	int2 operator+(const int2& _Other) const
	{
		return int2{ 
			this->x + _Other.x,
			this->y + _Other.y };
	}

	int2 operator-(const int2& _Other) const
	{
		return int2{
			this->x - _Other.x,
			this->y - _Other.y };
	}

	int2& operator+=(const int2& _Other)
	{
		this->x += _Other.x;
		this->y += _Other.y;

		return *this;
	}

	int2& operator-=(const int2& _Other)
	{
		this->x -= _Other.x;
		this->y -= _Other.y;

		return *this;
	}

	bool operator==(const int2& _Other) const
	{
		return (this->x == _Other.x && this->y == _Other.y);
	}

	bool operator!=(const int2& _Other) const
	{
		return (this->x != _Other.x && this->y != _Other.y);
	}

	static int2 Normalize(const int2& _Dir)
	{
		int2 ReturnValue = _Dir;
		ReturnValue.normalize();
		return ReturnValue;
	}

	void normalize()
	{		
		if (x > 0)
		{
			x = 1;
		}
		else if (x < 0)
		{
			x = -1;
		}

		if (y > 0)
		{
			y = 1;
		}
		else if (y < 0)
		{
			y = -1;
		}
	}
};

static float4 Lerp(const float4& _Start, const float4& _Dest, float _Time)
{
	float4 ReturnPos = _Start;
	ReturnPos += (_Dest - _Start) * _Time;
	return ReturnPos;
}

static int Lerp(int _Start, int _Dest, float _Time)
{
	int ReturnPos = _Start;
	ReturnPos += static_cast<int>((_Dest - _Start) * _Time);
	return ReturnPos;
}