#pragma once
#ifdef DirectX11Engein_EXPORTS
#define DirectX11Engein_API __declspec(dllexport) 
#else
#define DirectX11Engein_API __declspec(dllimport) 
#endif

#include "Camera.h"
#include "StaticMesh.h"
#include "DirectXText.h"
#include "Light.h"
#include "DirectInput.h"

namespace DirectX11Engein
{
	class DirectX11Engein
	{
	private:
		HWND hwnd;
		bool InitializeDirect3d11App();
		bool InitD2D_D3D101_DWrite();//for text
		bool InitScene();

		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11RenderTargetView* renderTargetView;
		int Width = 300;
		int Height = 300;
		float red = 0.f;
		float green = 0.f;
		float blue = 0.f;
		float alpha = 1.f;

		/*For Text*/
		ID3D10Device1 *d3d101Device;

		IDXGIAdapter1 *Adapter;

		/*int colormodr = 1;
		int colormodg = 1;
		int colormodb = 1;*/
		//std::map < std::string, StaticMesh> mp;
	public:

		/* ↓ Сделать private после всего ↓ */
		ID3D11Device* d3d11Device;
		ID3D11DeviceContext* d3d11DevCon;
		IDXGISwapChain* SwapChain;
		 /*Text*/
		ID3D11Texture2D *sharedTex11;
		IDXGIKeyedMutex *keyedMutex11;
		IDXGIKeyedMutex *keyedMutex10;
		ID2D1RenderTarget *D2DRenderTarget;
		ID2D1SolidColorBrush *Brush;

		explicit DirectX11Engein(int Width, int Height, HWND hwnd);
		explicit DirectX11Engein(HWND hwnd);//стандартный
		void SetBackgroundColor(float red, float green, float blue, float alpha);
		void UpdateScene();//Стандартная отрисовка
		void DrawScene();
		~DirectX11Engein();
	};
}

