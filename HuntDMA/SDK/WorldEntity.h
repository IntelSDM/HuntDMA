#pragma once
#include "Memory.h"
struct RenderNode {
	char pad_01[0x28]; // 0x00(0x28)
	unsigned __int64 rnd_flags; // 0x28(0x08)
	char pad_02[0xc]; // 0x30(0xc)
	unsigned int silhouettes_param; // 0x3c(0x04)
	char pad_03[0x8];
	float m_fWSMaxViewDist; //0x48
};
struct EntityNameStruct {
	char name[100];
};
enum class EntityType : int
{
	EnemyPlayer,
	FriendlyPlayer,
	Butcher,
	Spider,
	Zombie,
	CurrencyCollection,
	CashRegister,
	Immolator,
	Dog,
	WaterDevil,
	MeatHead,
	SupplyBox,
	AmmoBox,
	BearTrap,
	PoisonTrap,
	RotJaw,
	Assassin,
	ExtractionPoint,
	ScrapBeak,
	TripMine,
	Explosive,
	SpitzerBullets,
	PoisonBullets,
	HighVelocityBullets,
	IncendiaryBullets,
	DumDumBullets,
	ExplosiveBullets,
	FMJBullets,
	PoisonBolts,
	ExplosiveBolts,
	IncendiaryShells,
	PennyShotShells,
	FlechetteShells,
	SlugShells,
	AmmoCrate,
	SpecialAmmoCrate,
	AmmoBox,
	MediumAmmoPackage,
	ShotgunAmmoPackage,
	CompactAmmoPackage,
	LongAmmoPackage,
	Medkit,
};

class WorldEntity
{
private:
	uint64_t Class = 0x0;
	uint64_t ClassAddress = 0x0;
	uint64_t PosOffset = 0x128;
	uint64_t StringBufferOffset = 0x10;
	uint64_t ClassPointerOffset = 0x18;
	uint64_t SlotsPointerOffset = 0xA8;
	uint64_t SkeletonPoseOffset = 0xC00;
	uint64_t SlotOffset = 0x0;
	uint64_t RenderNodePointerOffset = 0xA8;
	RenderNode Node;
	EntityNameStruct EntityName;
	EntityNameStruct ClassName;

	uint64_t EntityNamePointer = 0x0;
	uint64_t ClassNamePointer = 0x0;
	uint64_t ClassPointer = 0x0;
	uint64_t SlotsPointer = 0x0;
	uint64_t SkeletonPose = 0x0;
	uint64_t Slot = 0x0;
	uint64_t RenderNodePointer = 0x0;
	Vector3 Position;
	EntityType Type;
	bool Valid = true;
	std::unordered_map<EntityType, std::wstring> Names = {
		{EntityType::EnemyPlayer, L"Enemy Player"},
		{EntityType::FriendlyPlayer, L"Friendly Player"},
		{EntityType::Butcher, L"Butcher"},
		{EntityType::Zombie, L"Zombie"},
		{EntityType::Dog, L"Dog"},
		{EntityType::Immolator, L"Immolator"},
		{EntityType::Spider, L"Spider"},
		{EntityType::WaterDevil, L"Water Devil"},
		{EntityType::MeatHead, L"Fat BOI"},
		{EntityType::SupplyBox, L"Supply Box"},
		{EntityType::AmmoBox, L"Ammo Box"},
		{EntityType::BearTrap, L"Bear Trap"},
		{EntityType::PoisonTrap, L"Poison Trap"},
		{EntityType::RotJaw, L"Rot Jaw"},
		{EntityType::Assassin, L"Assassin"},
		{EntityType::ScrapBeak, L"ScrapBeak"},
		{EntityType::TripMine, L"Trip Mine"},
		{EntityType::Explosive, L"Explosive"},
		{EntityType::SpitzerBullets, L"Spitzer Bullets"},
		{EntityType::PoisonBullets, L"Poison Bullets"},
		{EntityType::HighVelocityBullets, L"High Velocity Bullets"},
		{EntityType::IncendiaryBullets, L"Incendiary Bullets"},
		{EntityType::DumDumBullets, L"Dum Dum Bullets"},
		{EntityType::ExplosiveBullets, L"Explosive Bullets"},
		{EntityType::FMJBullets, L"FMJ Bullets"},
		{EntityType::PoisonBolts, L"Poison Bolts"},
		{EntityType::ExplosiveBolts, L"Explosive Bolts"},
		{EntityType::IncendiaryShells, L"Incendiary Shells"},
		{EntityType::PennyShotShells, L"Penny Shot Shells"},
		{EntityType::FlechetteShells, L"Flechette Shells"},
		{EntityType::SlugShells, L"Slug Shells"},
		{EntityType::AmmoCrate, L"Ammo Crate"},
		{EntityType::SpecialAmmoCrate, L"Special Ammo Crate"},
		{EntityType::MediumAmmoPackage, L"Medium Ammo Package"},
		{EntityType::ShotgunAmmoPackage, L"Shotgun Ammo Package"},
		{EntityType::CompactAmmoPackage, L"Compact Ammo Package"},
		{EntityType::LongAmmoPackage, L"Long Ammo Package"},
		{EntityType::Medkit, L"Medkit"},



	};
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
	void SetType(EntityType type) { Type = type; }
	EntityType GetType() { return Type; }
	void WriteNode(VMMDLL_SCATTER_HANDLE handle, int colour);
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	void UpdateNode(VMMDLL_SCATTER_HANDLE handle);
	void UpdateClass(VMMDLL_SCATTER_HANDLE handle);
	uint64_t GetClass() { return ClassAddress; }
	bool GetValid() { return Valid; }
	void SetValid(bool valid) { Valid = valid; }
	std::wstring GetName() { return Names[Type]; };
};