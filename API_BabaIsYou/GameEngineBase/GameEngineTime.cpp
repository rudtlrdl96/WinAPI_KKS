#include "GameEngineTime.h"

GameEngineTime::GameEngineTime()
{
	// 1�ʸ� ���������� ���� �����Դϴ�. 
	QueryPerformanceFrequency(&Second);
}

GameEngineTime::~GameEngineTime()
{
}

void GameEngineTime::TimeCheckStart()
{
	// 0�� 0�� 0�� 0��

	// ���� �� �Լ��� ��������� ���� �ð����� �� �Լ��� ������ ���������� ������ ��Ÿ�� ����.
	// ��ǻ�Ͱ� ����ִ� �׶����� �� �ð�.
	// 100�� ���Ծ��.
	QueryPerformanceCounter(&Prev);

	// 1�ʸ� 200���� ǥ���Ҽ� �ִ�.

		//QueryPerformanceCounter()

	// �������� �ð��� �� �غ� �Ҳ�
}


float GameEngineTime::TimeCheckEnd()
{
	// �׷��� �� �Լ��� ����ɶ����� �ɸ� �ð���....

	return 0.0f;
}