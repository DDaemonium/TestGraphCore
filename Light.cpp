#include "Light.h"

struct Light::Lightbuf
{
	Lightbuf()
	{
		ZeroMemory(this, sizeof(Lightbuf));
	}
	XMFLOAT3 dir;
	float pad1;
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
}light;

struct Light::cbPerFrame
{
	Lightbuf  light;
}constbuffPerFrame;

Light::Light(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon):d3d11Device(d3d11Device), d3d11DevCon(d3d11DevCon)
{
	light.dir = XMFLOAT3(0.f, 0.f, 0.f);//location
	light.ambient = XMFLOAT4(1.f, 1.f, 1.f, 1.f);//затемнённая часть
	light.diffuse = XMFLOAT4(0.f, 0.f, 0.f, 1.f);//свет
	light.pos = XMFLOAT3(0.0f, 0.0f, -1.0f);
	light.range = 1000.f;
	light.att = XMFLOAT3(0.0f, 0.2f, 0.0f);
	//light.check = 0.f;
	this->InitLight();
}

void Light::InitLight() {
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS_AMBIENT_LIGHT", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	this->d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
}

void Light::SetLocation(float x, float y, float z) {
	light.dir = XMFLOAT3(x, y, z);
}

void Light::SetAmbient(float red, float green, float blue, float alpha) {
	light.ambient = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDiffuse(float red, float green, float blue, float alpha) {
	light.diffuse = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetPosition(float x, float y, float z) {
	light.pos = XMFLOAT3(x, y, z);
}

void Light::SetAttenuation(float x, float y, float z) {
	light.att = XMFLOAT3(x, y, z);
}

void Light::SetRange(float range) {
	light.range = range;
}

void Light::SetShader(const CHAR *func_name) {
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, func_name, "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	this->d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
}

void Light::SetLight(){
	constbuffPerFrame.light = light;
	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
	this->d3d11DevCon->PSSetShader(PS, 0, 0);
}

Light::~Light()
{
	this->PS->Release();
	this->PS_Buffer->Release();
	this->cbPerFrameBuffer->Release();
}
