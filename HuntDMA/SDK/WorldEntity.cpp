#include "pch.h"
#include "WorldEntity.h"

WorldEntity::WorldEntity( uint64_t entity)
{
	this->Class = entity;
	this->ClassAddress = entity;
	
}
void WorldEntity::SetUp(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class + HpOffset1, &HpPointer1, sizeof(uint64_t));
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
	TargetProcess.AddScatterReadRequest(handle, this->HpPointer1 + HpOffset2, &HpPointer2, sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle,this->EntityNamePointer, &EntityName, sizeof(EntityNameStruct));
	TargetProcess.AddScatterReadRequest(handle, this->ClassPointer + StringBufferOffset,&ClassNamePointer, sizeof(uint64_t));
	if (SlotsPointer != 0)
		TargetProcess.AddScatterReadRequest(handle, this->SlotsPointer, &Slot, sizeof(uint64_t));
	else
		Slot = 0;
}
void WorldEntity::SetUp2(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->HpPointer2 + HpOffset3, &HpPointer3, sizeof(uint64_t));
	if (Slot != 0)
		TargetProcess.AddScatterReadRequest(handle, this->Slot + RenderNodePointerOffset, &RenderNodePointer, sizeof(uint64_t));
	else
		RenderNodePointer = 0;
	TargetProcess.AddScatterReadRequest( handle, this->ClassNamePointer,&ClassName, sizeof(EntityNameStruct));

}
void WorldEntity::SetUp3(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->HpPointer3 + HpOffset4, &HpPointer4, sizeof(uint64_t));
	ClassName.name[99] = '\0';
	EntityName.name[99] = '\0';
	TargetProcess.AddScatterReadRequest(handle, RenderNodePointer, &Node, sizeof(RenderNode));
}
void WorldEntity::SetUp4(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->HpPointer4 + HpOffset5, &HpPointer5, sizeof(uint64_t));
}

void WorldEntity::WriteNode(VMMDLL_SCATTER_HANDLE handle, int colour, bool show)
{
	uint32_t convertedcolour = 0;
	/*	std::vector<std::wstring>{LIT(L"Outline Red"), LIT(L"Outline Blue"), LIT(L"Outline Yellow"), LIT(L"Outline Orange"), LIT(L"Outline Cyan"), LIT(L"Outline White"),
	LIT(L"Filled Red"), LIT(L"Filled Blue"), LIT(L"Filled Yellow"), LIT(L"Filled Orange"), LIT(L"Filled Cyan"), LIT(L"Filled White")});*/
	if(colour == 0)
		convertedcolour = 0xFF0000FF; // Outline Red
	else if (colour == 1)
		convertedcolour = 0x0000FFFF; // Outline Blue
	else if (colour == 2)
		convertedcolour = 0xFFFF00FF; // Outline Yellow
	else if (colour == 3)
		convertedcolour = 0xFFA500FF; // Outline Orange
	else if (colour == 4)
		convertedcolour = 0x00FFFFFF; // Outline Cyan
	else if (colour == 5)
		convertedcolour = 0xFF00FFFF; // Outline Magenta
	else if (colour == 6)
		convertedcolour = 0xFFFFFFFF; // Outline White
	else if (colour == 7)
		convertedcolour = 0xFF000000; // Filled Red
	else if (colour == 8)
		convertedcolour = 0x0000FF00; // Filled Blue
	else if (colour == 9)
		convertedcolour = 0xFFFF0000; // Filled Yellow
	else if (colour == 10)
		convertedcolour = 0xFFA50000; // Filled Orange
	else if (colour == 11)
		convertedcolour = 0x00FFFF00; // Filled Cyan
	else if (colour == 12)
		convertedcolour = 0xFF00FF00; // Filled Magenta
	else if (colour == 13)
		convertedcolour = 0xFFFFFF00; // Filled White
	if (RenderNodePointer != 0)
	{
	//	if(Node.rnd_flags == 284558360584 || Node.rnd_flags == 9680453640)
		uint64_t allmap = 0x80018;
		uint64_t limit_distance = 0x80008;
		float maxdistance = 5000;
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x10, &allmap, sizeof(uint64_t)); // change render flag to max distance, allows us to use chams at further distances as long as the model isn't culled.
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x2C, &convertedcolour, sizeof(uint32_t));
		TargetProcess.AddScatterWriteRequest(handle, RenderNodePointer + 0x38, &maxdistance, sizeof(float));
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

void WorldEntity::UpdateHealth(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, HpPointer5, &Health, sizeof(HealthBar));
}

void WorldEntity::UpdateClass(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, this->Class, &ClassAddress, sizeof(uint64_t));
}