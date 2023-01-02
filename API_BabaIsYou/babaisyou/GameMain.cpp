#include <Windows.h>
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEnginePlatform/GameEngineWindow.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	GameEngineDebug::LeakCheck();

	GameEngineWindow::WindowCreate(hInstance, "MainWindow");

	GameEngineWindow::WindowLoop();

	// main이 끝나면 무조건 끝나기 때문에
	// 무조건 붙잡아둬야 합니다.

	return 1;
}