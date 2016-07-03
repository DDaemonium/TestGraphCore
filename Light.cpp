#include "Light.h"
struct Light::DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad; // Pad the last float so we can
}mDirLight;

struct Light::PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;
	// Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an
}mPointLight;

struct Light::SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;
	// Packed into 4D vector: (Direction,Spot)
	XMFLOAT3 Direction;
	float Spot;
	// Packed into 4D vector: (Att, Pad)
	XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an
			   // array of lights if we wanted.
}mSpotLight;

struct Light::Material
{
	Material() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
};

struct Light::cbPerFrame
{
	Material material;
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	XMFLOAT3 gEyePosW;
	float pad;
}constbuffPerFrame;


Light::Light(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon):d3d11Device(d3d11Device), d3d11DevCon(d3d11DevCon)
{
	mDirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	// Point light--position is changed every frame to animate
	// in UpdateScene function.
	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mPointLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	mPointLight.Range = 25.0f;
	// Spot light--position and direction changed every frame to
	// animate in UpdateScene function.
	mSpotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mSpotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	mSpotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight.Att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mSpotLight.Spot = 96.0f;
	mSpotLight.Range = 10000.0f;

	constbuffPerFrame.gEyePosW = XMFLOAT3(0.0f, 3.0f, -8.0f);

	constbuffPerFrame.material.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	constbuffPerFrame.material.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	constbuffPerFrame.material.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
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
	D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	this->d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
}

/*void Light::SetLocation(float x, float y, float z) {
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
*/
void Light::SetLight(){
	constbuffPerFrame.gDirLight = mDirLight;
	constbuffPerFrame.gPointLight = mPointLight;
	constbuffPerFrame.gSpotLight = mSpotLight;
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
