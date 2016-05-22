#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>
class DirectInput
{
private:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirInput;
public:
	DirectInput();
	/*bool InitDirectInput(HINSTANCE hInstance);
	void DetectInput(double time);*/
	~DirectInput();
};

