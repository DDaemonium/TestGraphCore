#pragma once
#include "StdaFx.h"


class StaticMesh {
private:
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA iinitData;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	D3D11_RASTERIZER_DESC wfdesc;
	D3D11_VIEWPORT viewport;

	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;

	ID3D11Buffer* squareIndexBuffer;
	ID3D11Buffer* squareVertBuffer;
	ID3D11VertexShader* VS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* D2D_PS_Buffer;
	ID3D11PixelShader* D2D_PS;
	ID3D11InputLayout* vertLayout;
	struct Vertex;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	UINT numElements;
	const int Width = 300;
	const int Height = 300;
	ID3D11RasterizerState* WireFrame;
	ID3D11BlendState* Transparency;

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	float scale[3] = {1.f, 1.f, 1.f};
	float rotaxis[4] = {0.0f, 1.0f, 0.0f, 0.0f};//default y
	float translation[3] = {0.0f, 0.0f, 0.0f};//default no translation
	float rotangle;
	std::vector<Vertex> vertices;
	LPCTSTR texture_name;

public:
	explicit StaticMesh(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon);
	bool InitScene();

	void SetRotationAngle(float angle);
	void SetScale(float ScaleX, float ScaleY, float ScaleZ);
	void SetRotationAxis(float x, float y, float z, float w);
	void SetTranslation(float OffsetX, float OffsetY, float OffsetZ);

	float GetRotationAngle();
	float *GetScale();
	float *GetRotationAxis();
	float *GetTranslation();
	void SetTextureName(const TCHAR *texture_name);
	void Draw();
	~StaticMesh();
};