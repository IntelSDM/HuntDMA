#pragma once
class Camera
{
private:
	uint64_t CameraBase = 0x270;
	uint64_t ViewMatrixOffset = 0x230;
	uint64_t CameraPosOffset = 0x2F0;
	uint64_t ProjectionMatrixOffset = 0x270;

	Vector3 Position;
	ViewMatrix RenderMatrix;
	ViewMatrix ProjectionMatrix;

public:
	void UpdateCamera(VMMDLL_SCATTER_HANDLE handle);
	Vector3 GetPosition() { return Position;}
	Vector2 WorldToScreen(Vector3 pos);

};