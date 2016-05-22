#include "DirectX11Engein.h"


namespace DirectX11Engein {

	DirectX11Engein::DirectX11Engein(int Width, int Height, HWND hwnd)
	{
		this->Width = Width;
		this->Height = Height;
		this->hwnd = hwnd;
		if (!InitializeDirect3d11App())    //Инициализация DirectX
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
		}

		if (!InitScene())    //Инициализация сцены
		{
			MessageBox(0, L"Scene Initialization - Failed",
				L"Error", MB_OK);
		}
		if (!InitD2D_D3D101_DWrite())    //Инициализация DirectX10 для работы с текстом
		{
			MessageBox(0, L"DirectX10 Initialization - Failed",
				L"Error", MB_OK);
		}

	}

	DirectX11Engein::DirectX11Engein(HWND hwnd)
	{
		this->hwnd = hwnd;
		if (!InitializeDirect3d11App())    //Инициализация DirectX
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
		}

		if (!InitScene())    //Инициализация сцены
		{
			MessageBox(0, L"Scene Initialization - Failed",
				L"Error", MB_OK);
		}
		if (!InitD2D_D3D101_DWrite())    //Инициализация DirectX10 для работы с текстом
		{
			MessageBox(0, L"DirectX10 Initialization - Failed",
				L"Error", MB_OK);
		}
	}

	bool DirectX11Engein::InitializeDirect3d11App() // инициализация DirectX 11
	{
		//Describe our SwapChain Buffer
		DXGI_MODE_DESC bufferDesc;

		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//Describe our SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE; //full screen (false)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Create DXGI factory to enumerate adapters///////////////////////////////////////////////////////////////////////////
		IDXGIFactory1 *DXGIFactory;

		CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);

		// Use the first adapter    

		DXGIFactory->EnumAdapters1(0, &Adapter);

		DXGIFactory->Release();

		//Create our Direct3D 11 Device and SwapChain//////////////////////////////////////////////////////////////////////////
		D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);


		//Create our SwapChain
		 D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

		//Create our BackBuffer
		ID3D11Texture2D* BackBuffer;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

		//Create our Render Target
		d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
		BackBuffer->Release();

		//Describe our Depth/Stencil Buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = Width;
		depthStencilDesc.Height = Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//Create the Depth/Stencil View
		d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
		d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

		//Set our Render Target
		d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		return true;
	}

	bool DirectX11Engein::InitD2D_D3D101_DWrite()
	{
		//Create our Direc3D 10.1 Device///////////////////////////////////////////////////////////////////////////////////////
		D3D10CreateDevice1(this->Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT,
			D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device);

		//Create Shared Texture that Direct3D 10.1 will render on//////////////////////////////////////////////////////////////
		D3D11_TEXTURE2D_DESC sharedTexDesc;

		ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

		sharedTexDesc.Width = Width;
		sharedTexDesc.Height = Height;
		sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sharedTexDesc.MipLevels = 1;
		sharedTexDesc.ArraySize = 1;
		sharedTexDesc.SampleDesc.Count = 1;
		sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
		sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	    d3d11Device->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);

		// Get the keyed mutex for the shared texture (for D3D11)///////////////////////////////////////////////////////////////
		sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);

		// Get the shared handle needed to open the shared texture in D3D10.1///////////////////////////////////////////////////
		IDXGIResource *sharedResource10;
		HANDLE sharedHandle10;

		sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

		sharedResource10->GetSharedHandle(&sharedHandle10);

		sharedResource10->Release();

		// Open the surface for the shared texture in D3D10.1///////////////////////////////////////////////////////////////////
		IDXGISurface1 *sharedSurface10;

		d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

		sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

		// Create D2D factory///////////////////////////////////////////////////////////////////////////////////////////////////
		ID2D1Factory *D2DFactory;
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

		ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

		renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
		renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

		D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

		sharedSurface10->Release();
		D2DFactory->Release();

		// Create a solid color brush to draw something with        
		D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f), &Brush);

		//DirectWrite/////////////////change after release

		////////////////////////////////////////////////////////////////////////////////////////////
		d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

		//Release the Adapter interface
		Adapter->Release();

		return true;
	}


	bool DirectX11Engein::InitScene()
	{

		return true;
	}

	void DirectX11Engein::SetBackgroundColor(float red, float green, float blue, float alpha)//переррисовка сцены
	{
		//Update the colors of our scene
		this->red = red;
		this->green = green;
		this->blue = blue;
	}

	void DirectX11Engein::UpdateScene()//переррисовка сцены
	{
           
	}


	void DirectX11Engein::DrawScene()//Отрисовка сцены
	{
		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(this->red, this->green, this->blue, this->alpha);
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//Present the backbuffer to the screen
		//SwapChain->Present(0, 0); вернуть не удалять!
	}


	DirectX11Engein::~DirectX11Engein()
	{
		//Release the COM Objects we created
		SwapChain->Release();
		d3d11Device->Release();
		d3d11DevCon->Release();
		renderTargetView->Release();
		depthStencilView->Release();
		depthStencilBuffer->Release();

		d3d101Device->Release();
		keyedMutex11->Release();
		keyedMutex10->Release();
		D2DRenderTarget->Release();
		Brush->Release();
		sharedTex11->Release();
	}
}
