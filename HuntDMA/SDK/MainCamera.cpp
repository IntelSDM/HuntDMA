#include "Pch.h"
#include "MainCamera.h"

MainCamera::MainCamera()
{
	StaticClass = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameAssembly.dll") + StaticClass);
	StaticField = TargetProcess.Read<uint64_t>(StaticClass + StaticField);

	/*
		[Token(Token = "0x400134F")]
	[Il2CppDummyDll.FieldOffset(Offset = "0x8C")]
	public static int \u0303\u0301\u0305\u0305\u0301\u0301\u0307\u0303\u0305\u0306\u0305\u0304\u0301\u0308\u0306;

	// Token: 0x04001350 RID: 4944
	[Token(Token = "0x4001350")]
	[Il2CppDummyDll.FieldOffset(Offset = "0x90")]
	public static int \u0307\u0305\u0304\u0303\u0308\u0302\u0303\u0307\u0302\u0306\u0305\u0304\u0307\u0307\u0307;
	look for 2 static ints like this
	*/
	Width = TargetProcess.Read<int>(StaticField + CameraWidth); 
	Height = TargetProcess.Read<int>(StaticField + CameraHeight);
}

void MainCamera::UpdateViewMatrix()
{
	Matrix = TargetProcess.Read<ViewMatrix>(StaticField + ViewMatrixOffset);

}
ViewMatrix MainCamera::GetViewMatrix()
{
	return Matrix;
}

Vector2 MainCamera::WorldToScreen(Vector3 position)
{
	Vector3 transform(Matrix.matrix[0][3], Matrix.matrix[1][3], Matrix.matrix[2][3]);
	Vector3 right(Matrix.matrix[0][0], Matrix.matrix[1][0], Matrix.matrix[2][0]);
	Vector3 up(Matrix.matrix[0][1], Matrix.matrix[1][1], Matrix.matrix[2][1]);

	float w = Vector3::Dot(transform, position) + Matrix.matrix[3][3];

	if (w < 1.f)
		return Vector2(0, 0);
	float x = Vector3::Dot(right, position) + Matrix.matrix[3][0];
	float y = Vector3::Dot(up, position) + Matrix.matrix[3][1];

	return  Vector2((Width / 2) * (1.f + x / w), (Height / 2) * (1.f - y / w));
}