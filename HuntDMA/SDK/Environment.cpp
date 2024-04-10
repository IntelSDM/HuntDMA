#include "Pch.h"
#include "Environment.h"
#include "WorldEntity.h"
#include "Globals.h"
Environment::Environment()
{
	SystemGlobalEnvironment = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameHunt.dll") + SystemGlobalEnvironment);
	printf(LIT("SystemGlobalEnvironment: 0x%X\n"), SystemGlobalEnvironment);
	EntitySystem = TargetProcess.Read<uint64_t>(SystemGlobalEnvironment + EntitySystem);
	printf(LIT("EntitySystem: 0x%X\n"), EntitySystem);
	pSystem = TargetProcess.Read<uint64_t>(SystemGlobalEnvironment + pSystem);
	printf(LIT("pSystem: 0x%X\n"), pSystem);

}

void Environment::GetEntitys()
{
	ObjectCount = TargetProcess.Read<uint16_t>(EntitySystem + ObjectCountOffset);
	printf(LIT("ObjectCount: %d\n"), ObjectCount);
	EntityList = EntitySystem + EntityListOffset;
}

void Environment::CacheEntities()
{
	GetEntitys();
	std::vector<uint64_t> entitylist;
	entitylist.resize(ObjectCount);
	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(ObjectCount);
	TargetProcess.Read(EntityList, object_raw_ptr.get(), ObjectCount * sizeof(uint64_t));
	for (size_t i = 0; i < ObjectCount; i++)
	{
		entitylist[i] = object_raw_ptr[i];
	}
	std::vector<std::shared_ptr<WorldEntity>> entitypointerlist;
	auto handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < ObjectCount; i++)
	{
		uint64_t entity = entitylist[i];
		
		if (entity == NULL) {
			continue;
		}
		printf(LIT("Entity: 0x%X\n"), entity);
		entitypointerlist.push_back(std::make_shared<WorldEntity>(entity));

	}
	TargetProcess.ExecuteReadScatter(handle);

	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp(handle);

	}
	TargetProcess.ExecuteReadScatter(handle);

	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp1(handle);

	}
	TargetProcess.ExecuteReadScatter(handle);

	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp2(handle);
		

	}
	TargetProcess.ExecuteReadScatter(handle);

	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp3(handle);

	}
	TargetProcess.ExecuteReadScatter(handle);
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->WriteNode(handle);

	}
	TargetProcess.ExecuteWriteScatter(handle);
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		printf(LIT("Entity Position: %f %f %f\n"), ent->GetPosition().x, ent->GetPosition().y, ent->GetPosition().z);
		printf(LIT("Entity ClassName: %s\n"), ent->GetEntityName().name);
		printf(LIT("Entity Class: %s\n"), ent->GetEntityClassName().name);
		printf(LIT("Entity Silhouettes: %d\n"), ent->GetRenderNode().silhouettes_param);
		Vector2 screenpos = CameraInstance->WorldToScreen(ent->GetPosition());
		printf(LIT("Entity Screen Position: %f %f\n"), screenpos.x, screenpos.y);

	}
}