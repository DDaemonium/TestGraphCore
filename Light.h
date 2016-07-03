#pragma once
#include "StdaFx.h"

class Light
{
private:
	ID3D11Buffer* cbPerFrameBuffer;
	D3D11_BUFFER_DESC cbbd;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11PixelShader* PS;
	ID3D10Blob* PS_Buffer;
	struct DirectionalLight;
	struct PointLight;
	struct SpotLight;
	struct Material;
	struct cbPerFrame;
public:
	Light(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon);
	void InitLight();
	/*void SetLocation(float x, float y, float z);
	void SetAmbient(float red, float green, float blue, float alpha);
	void SetDiffuse(float red, float green, float blue, float alpha);
	void SetPosition(float x, float y, float z);
	void SetAttenuation(float x, float y, float z);
	void SetRange(float range);
	void SetShader(const CHAR *func_name);*/
	void SetLight();
	~Light();
};

