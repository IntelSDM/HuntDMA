#include "pch.h"
#include "WorldEntity.h"

WorldEntity::WorldEntity( uint64_t entity)
{
	this->Class = entity;

}
void WorldEntity::SetUp(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class + PosOffset, &Position, sizeof(Vector3));
	TargetProcess.AddScatterReadRequest(handle, this->Class + StringBufferOffset, &EntityNamePointer, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, this->Class + ClassPointerOffset,&ClassPointer, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, this->Class + SlotsPointerOffset, &SlotsPointer, sizeof(uint64_t));
}
void WorldEntity::SetUp1(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle,this->EntityNamePointer, &EntityName, sizeof(EntityNameStruct));
	TargetProcess.AddScatterReadRequest(handle, this->ClassPointer + StringBufferOffset,&ClassNamePointer, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, this->SlotsPointer, &Slot, sizeof(uint64_t));
}
void WorldEntity::SetUp2(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Slot + RenderNodePointerOffset, &RenderNodePointer, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest( handle, this->ClassNamePointer,&ClassName, sizeof(EntityNameStruct));

}
void WorldEntity::SetUp3(VMMDLL_SCATTER_HANDLE handle)
{
	ClassName.name[99] = '\0';
	EntityName.name[99] = '\0';
	TargetProcess.AddScatterReadRequest(handle, RenderNodePointer, &Node, sizeof(RenderNode));
}

void WorldEntity::WriteNode(VMMDLL_SCATTER_HANDLE handle)
{
	uint32_t RGBAColor = 0xFF000000;
	if (RenderNodePointer != 0)
	{
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x3c, &RGBAColor, sizeof(uint32_t));
	}
}