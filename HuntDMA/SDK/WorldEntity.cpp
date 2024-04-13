#include "pch.h"
#include "WorldEntity.h"

WorldEntity::WorldEntity( uint64_t entity)
{
	this->Class = entity;
	this->ClassAddress = entity;
	
}
void WorldEntity::SetUp(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class + PosOffset, &Position, sizeof(Vector3));
	TargetProcess.AddScatterReadRequest(handle, this->Class + StringBufferOffset, &EntityNamePointer, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, this->Class + ClassPointerOffset,&ClassPointer, sizeof(uint64_t));
	if (Class != 0)
	TargetProcess.AddScatterReadRequest(handle, this->Class + SlotsPointerOffset, &SlotsPointer, sizeof(uint64_t));
		else
		SlotsPointer = 0;
}
void WorldEntity::SetUp1(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle,this->EntityNamePointer, &EntityName, sizeof(EntityNameStruct));
	TargetProcess.AddScatterReadRequest(handle, this->ClassPointer + StringBufferOffset,&ClassNamePointer, sizeof(uint64_t));
	if (SlotsPointer != 0)
	TargetProcess.AddScatterReadRequest(handle, this->SlotsPointer, &Slot, sizeof(uint64_t));
	else
		Slot = 0;
}
void WorldEntity::SetUp2(VMMDLL_SCATTER_HANDLE handle)
{
	if (Slot != 0)
		TargetProcess.AddScatterReadRequest(handle, this->Slot + RenderNodePointerOffset, &RenderNodePointer, sizeof(uint64_t));
	else
		RenderNodePointer = 0;
	TargetProcess.AddScatterReadRequest( handle, this->ClassNamePointer,&ClassName, sizeof(EntityNameStruct));

}
void WorldEntity::SetUp3(VMMDLL_SCATTER_HANDLE handle)
{
	ClassName.name[99] = '\0';
	EntityName.name[99] = '\0';
	TargetProcess.AddScatterReadRequest(handle, RenderNodePointer, &Node, sizeof(RenderNode));
}

void WorldEntity::WriteNode(VMMDLL_SCATTER_HANDLE handle,int colour)
{
	uint32_t convertedcolour = 0;
	/*	std::vector<std::wstring>{LIT(L"Outline Red"), LIT(L"Outline Blue"), LIT(L"Outline Yellow"), LIT(L"Outline Orange"), LIT(L"Outline Cyan"), LIT(L"Outline White"),
	LIT(L"Filled Red"), LIT(L"Filled Blue"), LIT(L"Filled Yellow"), LIT(L"Filled Orange"), LIT(L"Filled Cyan"), LIT(L"Filled White")});*/
	if(colour == 0)
		convertedcolour = 0xFF0000FF;
	else if (colour == 1)
		convertedcolour = 0x0000FFFF;
	else if (colour == 2)
		convertedcolour = 0xFFFF00FF;
	else if (colour == 3)
		convertedcolour = 0xFFA500FF;
	else if (colour == 4)
		convertedcolour = 0x00FFFFFF;
	else if (colour == 5)
		convertedcolour = 0xFFFFFFFF;
	else if (colour == 6)
		convertedcolour = 0xFF000000;
	else if (colour == 7)
		convertedcolour = 0x0000FF00;
	else if (colour == 8)
		convertedcolour = 0xFFFF0000;
	else if (colour == 9)
		convertedcolour = 0xFFA50000;
	else if (colour == 10)
		convertedcolour = 0x00FFFF00;
	else if (colour == 11)
		convertedcolour = 0xFFFFFF00;
	if (RenderNodePointer != 0)
	{
	//	if(Node.rnd_flags == 284558360584 || Node.rnd_flags == 9680453640)
		uint64_t allmap = 0x80018;
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x28, &allmap, sizeof(uint64_t)); // change render flag to max distance, allows us to use chams at further distances as long as the model isn't culled.
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x3c, &convertedcolour, sizeof(uint32_t));
	}
}

void WorldEntity::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class + PosOffset, &Position, sizeof(Vector3));
}

void WorldEntity::UpdateNode(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, RenderNodePointer, &Node, sizeof(RenderNode));
}

void WorldEntity::UpdateClass(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class, &ClassAddress, sizeof(uint64_t));
}