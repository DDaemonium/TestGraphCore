#pragma once
#include "StdaFx.h"



class Camera
{
private:
	D3D11_BUFFER_DESC cbbd;

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;

	XMMATRIX rotation;
	XMMATRIX scale;
	XMMATRIX translation;
	XMVECTOR rotaxis;
	float rotangle;
	ID3D11Buffer* cbPerObjectBuffer;
	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX  World;
	} cbPerObj;

public:
	explicit Camera();
	bool InitCamera(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon);
	void SetCamPosition(float x, float y, float z, float w);
	void SetCamTarget(float x, float y, float z, float w);
	void SetCamUp(float x, float y, float z, float w);
	void SetCamProjection(float FovAngleY, float AspectRatio, float NearZ, float FarZ);

	void SetRotationAngle(float angle);
	void SetScale(float ScaleX, float ScaleY, float ScaleZ);
	void SetRotationAxis(float x, float y, float z, float w);
	void SetTranslation(float OffsetX, float OffsetY, float OffsetZ);
	void SetScale(float *scale);
	void SetRotationAxis(float *axis);
	void SetTranslation(float *offset);


	void SetWVPMatrix();
	void SetWVPMatrix(float Rotangle);
	void SetTextWVPMatrix();
	~Camera();
};

