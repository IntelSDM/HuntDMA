#include "Pch.h"
#include "Camera.h"
#include "Globals.h"
void Camera::UpdateCamera(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, EnvironmentInstance->GetpSystem() +  CameraBase + CameraPosOffset,&Position,sizeof(Vector3));
	TargetProcess.AddScatterReadRequest(handle, EnvironmentInstance->GetpSystem() + CameraBase + ViewMatrixOffset, &RenderMatrix, sizeof(ViewMatrix));
	TargetProcess.AddScatterReadRequest(handle, EnvironmentInstance->GetpSystem() + CameraBase + ProjectionMatrixOffset, &ProjectionMatrix, sizeof(ViewMatrix));
}

Vector2 Camera::WorldToScreen(Vector3 pos)
{
	if(Position == Vector3::Zero())
		return Vector2::Zero();
	Vector3 vAxisX, vAxisY, vAxisZ;
	Vector2 ScreenLocation;
	Vector4 transformed, projected;
	int pViewport[4] = { 0, 0, 2560, 1440 };

	transformed.x = (float)(pos.x * RenderMatrix.matrix[0][0]) + (float)(pos.y * RenderMatrix.matrix[1][0]) + (float)(pos.z * RenderMatrix.matrix[2][0]) + RenderMatrix.matrix[3][0];
	transformed.y = (float)(pos.x * RenderMatrix.matrix[0][1]) + (float)(pos.y * RenderMatrix.matrix[1][1]) + (float)(pos.z * RenderMatrix.matrix[2][1]) + RenderMatrix.matrix[3][1];
	transformed.z = (float)(pos.x * RenderMatrix.matrix[0][2]) + (float)(pos.y * RenderMatrix.matrix[1][2]) + (float)(pos.z * RenderMatrix.matrix[2][2]) + RenderMatrix.matrix[3][2];
	transformed.w = (float)(pos.x * RenderMatrix.matrix[0][3]) + (float)(pos.y * RenderMatrix.matrix[1][3]) + (float)(pos.z * RenderMatrix.matrix[2][3]) + RenderMatrix.matrix[3][3];

	bool visible = transformed.z < 0.0f;

	projected.x = (float)(transformed.x * ProjectionMatrix.matrix[0][0]) + (float)(transformed.y * ProjectionMatrix.matrix[1][0]) + (float)(transformed.z * ProjectionMatrix.matrix[2][0]) + (float)(transformed.w * ProjectionMatrix.matrix[3][0]);
	projected.y = (float)(transformed.x * ProjectionMatrix.matrix[0][1]) + (float)(transformed.y * ProjectionMatrix.matrix[1][1]) + (float)(transformed.z * ProjectionMatrix.matrix[2][1]) + (float)(transformed.w * ProjectionMatrix.matrix[3][1]);
	projected.z = (float)(transformed.x * ProjectionMatrix.matrix[0][2]) + (float)(transformed.y * ProjectionMatrix.matrix[1][2]) + (float)(transformed.z * ProjectionMatrix.matrix[2][2]) + (float)(transformed.w * ProjectionMatrix.matrix[3][2]);
	projected.w = (float)(transformed.x * ProjectionMatrix.matrix[0][3]) + (float)(transformed.y * ProjectionMatrix.matrix[1][3]) + (float)(transformed.z * ProjectionMatrix.matrix[2][3]) + (float)(transformed.w * ProjectionMatrix.matrix[3][3]);

	if (projected.z == 0.0f)
	{
		ScreenLocation.x = 0;
		ScreenLocation.y = 0;
	//	ScreenLocation.z = 0;
		return ScreenLocation;
	}

	projected.x /= projected.w;
	projected.y /= projected.w;
	projected.z /= projected.w;

	visible = visible && (abs(projected.x) <= 1.0f) && (abs(projected.y) <= 1.0f);

	if (!visible) {
		ScreenLocation.x = 0;
		ScreenLocation.y = 0;
	//	ScreenLocation.z = 0;
		return ScreenLocation;
	}

	//output coords
	ScreenLocation.x = pViewport[0] + (1 + projected.x) * pViewport[2] / 2;
	ScreenLocation.y = pViewport[1] + (1 - projected.y) * pViewport[3] / 2;  //flip coords for y axis
//	ScreenLocation.z = projected.z;
	return ScreenLocation;
}
