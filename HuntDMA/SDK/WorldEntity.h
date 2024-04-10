#pragma once
#include "Memory.h"
struct RenderNode {
	char pad_01[0x28]; // 0x00(0x28)
	unsigned __int64 rnd_flags; // 0x28(0x08)
	char pad_02[0xc]; // 0x30(0xc)
	unsigned int silhouettes_param; // 0x3c(0x04)
};
struct EntityNameStruct {
	char name[100];
};
class WorldEntity
{
private:
	uint64_t Class = 0x0;
	uint64_t PosOffset = 0x128;
	uint64_t StringBufferOffset = 0x10;
	uint64_t ClassPointerOffset = 0x18;
	uint64_t SlotsPointerOffset = 0xA8;
	uint64_t SlotOffset = 0x0;
	uint64_t RenderNodePointerOffset = 0xA8;
	RenderNode Node;
	EntityNameStruct EntityName;
	EntityNameStruct ClassName;

	uint64_t EntityNamePointer = 0x0;
	uint64_t ClassNamePointer = 0x0;
	uint64_t ClassPointer = 0x0;
	uint64_t SlotsPointer = 0x0;
	uint64_t Slot = 0x0;
	uint64_t RenderNodePointer = 0x0;
	Vector3 Position;
public:
	WorldEntity( uint64_t classptr);
	void SetUp(VMMDLL_SCATTER_HANDLE handle);
	void SetUp1(VMMDLL_SCATTER_HANDLE handle);
	void SetUp2(VMMDLL_SCATTER_HANDLE handle);
	void SetUp3(VMMDLL_SCATTER_HANDLE handle);
	EntityNameStruct GetEntityName() { return EntityName; }	
	EntityNameStruct GetEntityClassName() { return ClassName; }
	Vector3 GetPosition() { return Position; }
	RenderNode GetRenderNode() { return Node; }
	void WriteNode(VMMDLL_SCATTER_HANDLE handle);
};