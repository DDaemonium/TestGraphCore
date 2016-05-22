#pragma once

#include "StdaFx.h"
#include "Camera.h"

class DirectXText
{
private:
	struct Vertex;

	ID3D11Buffer *d2dVertBuffer;
	ID3D11Buffer *d2dIndexBuffer;
	ID3D11ShaderResourceView *d2dTexture;
	std::wstring printText;
	IDWriteFactory *DWriteFactory;
	IDWriteTextFormat *TextFormat;
	ID3D11SamplerState* CubesTexSamplerState;
	ID3D11RasterizerState* CWcullMode;

	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11Texture2D *sharedTex11;
	IDXGIKeyedMutex *keyedMutex11;
	IDXGIKeyedMutex *keyedMutex10;
	ID2D1RenderTarget *D2DRenderTarget;
	ID2D1SolidColorBrush *Brush;
	float top;
	float left;
	float right;
	float bottom;
	float r;//red
	float g;//green
	float b;//blue
	float a;//alpha
	ID3D11BlendState* Transparency;
public:
	explicit DirectXText(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D11Texture2D *sharedTex11, IDXGIKeyedMutex *keyedMutex11, IDXGIKeyedMutex *keyedMutex10, ID2D1RenderTarget *D2DRenderTarget, ID2D1SolidColorBrush *Brush);
	void InitD2DScreenTexture();
	void SetFontFormat(const WCHAR * fontFamily = L"Script", float fontSize = 12.f, DWRITE_FONT_WEIGHT Weight = DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE fontStile = DWRITE_FONT_STYLE_NORMAL);//доработать позже добавить полную настройку стилей
	void RenderText(std::wstring text, Camera *camera);
	void SetPosition(float left, float top, float right, float bottom);
	void SetFontColor(float r, float g, float b, float a);
	~DirectXText();
};

