
//Include and link appropriate libraries and StaticMeshs//
//#include "StaticMesh.h"
#include "DirectX11Engein.h"
#include <memory>


//Global Declarations - Others//
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;
HRESULT hr;
float rot = 0.01;
const int Width = 300;
const int Height = 300;

float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;

float position = 0.f;
int posmod = 1;
//Function Prototypes//
void DrawScene();
void UpdateScene();

bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed);
int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

//Vertex Structure and Vertex Layout (Input Layout)//



Camera camera;
std::unique_ptr<DirectX11Engein::DirectX11Engein> DXEngein;
std::unique_ptr<DirectXText> Text;
std::unique_ptr<StaticMesh> Stat_Mesh;
std::unique_ptr<StaticMesh> Stat_Mesh2;
std::unique_ptr<Light> light;
std::unique_ptr<Light> light2;

int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{

	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}
	DXEngein = std::make_unique < DirectX11Engein::DirectX11Engein>(Width, Height, hwnd);
	Text = std::make_unique<DirectXText>(DXEngein->d3d11Device, DXEngein->d3d11DevCon, DXEngein->sharedTex11, DXEngein->keyedMutex11, DXEngein->keyedMutex10, DXEngein->D2DRenderTarget, DXEngein->Brush);
	Stat_Mesh = std::make_unique <StaticMesh>(DXEngein->d3d11Device, DXEngein->d3d11DevCon);
	Stat_Mesh2 = std::make_unique <StaticMesh>(DXEngein->d3d11Device, DXEngein->d3d11DevCon);
	light = std::make_unique<Light>(DXEngein->d3d11Device, DXEngein->d3d11DevCon);
	light2 = std::make_unique<Light>(DXEngein->d3d11Device, DXEngein->d3d11DevCon);
	//light2->SetShader("PS_AMBIENT_LIGHT");
	Stat_Mesh2->SetTextureName(L"braynzar.jpg");
	//light2->SetLocation(-1.0f, 1.0f, 0.0f);
	//light2->SetAmbient(0.15f, 0.15f, 0.15f, 1.0f);
	//light2->SetDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	//light2->SetShader("PS_POINT_LIGHT");
	if (!camera.InitCamera(DXEngein->d3d11Device, DXEngein->d3d11DevCon))	//Initialize our camera
	{
		MessageBox(0, L"Camera Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}
	messageloop();
	return 0;
}

bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed)
{
	typedef struct _WNDCLASS {
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndProc;
		int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(
		NULL,
		WndClassName,
		L"Test Project",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}

void UpdateScene() {
	rot += .0005f;
	if (rot > 6.28f)
		rot = 0.0f;
	Stat_Mesh->SetRotationAngle(rot);
	Stat_Mesh2->SetRotationAngle(rot);
	Stat_Mesh->SetRotationAxis(1.f, 0.f, 0.f, 0.f);
	Stat_Mesh2->SetTranslation(2.f, 2.f, -3.f);

	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
	Text->SetFontColor(red, green, blue, 1.f);

	position += posmod*0.5;
	if (position >= 200 || position <= 0) {
		posmod *= -1;
	}
	Text->SetPosition(0, position, 100, 30);
}

void DrawScene()
{
	DXEngein->DrawScene();
	/* ↓ Draw Light ↓ */
	

	//light->SetLight();
	/* SetWVPmatrix */
	camera.SetRotationAxis(Stat_Mesh->GetRotationAxis());
	camera.SetTranslation(Stat_Mesh->GetTranslation());
	camera.SetWVPMatrix(Stat_Mesh->GetRotationAngle());
	//Draw the triangle
	Stat_Mesh->Draw();
	
	camera.SetRotationAxis(Stat_Mesh2->GetRotationAxis());
	camera.SetTranslation(Stat_Mesh2->GetTranslation());
	camera.SetWVPMatrix(Stat_Mesh2->GetRotationAngle());
	//Draw the triangle
	light2->SetLight();
	Stat_Mesh2->Draw();
	Text->RenderText(L"Привет Мир!", &camera);
	
	//Present the backbuffer to the screen
	DXEngein->SwapChain->Present(0, 0);
}

int messageloop() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		BOOL PeekMessageL(
			LPMSG lpMsg,
			HWND hWnd,
			UINT wMsgFilterMin,
			UINT wMsgFilterMax,
			UINT wRemoveMsg
			);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// run game code            
			UpdateScene();
			DrawScene();
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

