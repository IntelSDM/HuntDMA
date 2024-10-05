#pragma once
#include "Memory.h"
struct RenderNode {
	char pad_01[0x10]; // 0x00(0x10)
	unsigned __int64 rnd_flags; // 0x10(0x08)
	char pad_02[0x14]; // 0x18(0x14)
	unsigned int silhouettes_param; // 0x2C(0x04)
	char pad_03[0x8]; // 0x30(0x8)
	float m_fWSMaxViewDist; //0x38(0x4)
};
struct HealthBar {
	char pad_01[0x10]; // 0x00(0x10)
	unsigned int current_hp; // 0x10(0x04)
	unsigned int regenerable_max_hp; // 0x14(0x04)
	unsigned int current_max_hp; // 0x18(0x04)
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
	Assassin,
	Scrapbeak,
	Rotjaw,
	Hellborn,

	CurrencyCollection,
	CashRegister,
	GoldCashRegister,

	ExtractionPoint,

	BearTrap,
	TripMine,

	Barrel,
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

	SupplyBox,
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
	uint64_t PosOffset = 0x134;
	uint64_t StringBufferOffset = 0x10;
	uint64_t ClassPointerOffset = 0x18;
	uint64_t SlotsPointerOffset = 0xA8;
	uint64_t SlotOffset = 0x0;
	uint64_t RenderNodePointerOffset = 0xA0;
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
	EntityType Type;

	uint64_t HpOffset1 = 0x198;
	uint64_t HpOffset2 = 0x20;
	uint64_t HpOffset3 = 0xB0;
	uint64_t HpOffset4 = 0x78;
	uint64_t HpOffset5 = 0x58;
	uint64_t HpPointer1 = 0x0;
	uint64_t HpPointer2 = 0x0;
	uint64_t HpPointer3 = 0x0;
	uint64_t HpPointer4 = 0x0;
	uint64_t HpPointer5 = 0x0;
	HealthBar Health;

	bool Valid = true;
	std::unordered_map<EntityType, std::wstring> Names = {
		{EntityType::EnemyPlayer, L"Enemy Player"},
		{EntityType::FriendlyPlayer, L"Teammate"},

		{EntityType::Butcher, L"Butcher"},
		{EntityType::Assassin, L"Assassin"},
		{EntityType::Scrapbeak, L"Screapbeak"},
		{EntityType::Spider, L"Spider"},
		{EntityType::Rotjaw, L"Rotjaw"},
		{EntityType::Hellborn, L"Hellborn"},

		{EntityType::CurrencyCollection, L"BB"},
		{EntityType::CashRegister, L"Cash Register"},
		{EntityType::GoldCashRegister, L"GOLD Cash Register"},

		{EntityType::ExtractionPoint, L"Extraction"},

		{EntityType::BearTrap, L"Bear Trap"},
		{EntityType::TripMine, L"Trip Mine"},

		{EntityType::Barrel, L"Barrel"},

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

		{EntityType::SupplyBox, L"Supply Box"},
		{EntityType::AmmoCrate, L"Ammo Crate"},
		{EntityType::SpecialAmmoCrate, L"Special Ammo Crate"},
		{EntityType::AmmoBox, L"Ammo Box"},
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
	void SetUp4(VMMDLL_SCATTER_HANDLE handle);
	EntityNameStruct GetEntityName() { return EntityName; }
	EntityNameStruct GetEntityClassName() { return ClassName; }
	Vector3 GetPosition() { return Position; }
	RenderNode GetRenderNode() { return Node; }
	HealthBar GetHealth() { return Health; }
	void SetType(EntityType type) { Type = type; }
	EntityType GetType() { return Type; }
	void WriteNode(VMMDLL_SCATTER_HANDLE handle, int colour, bool show);
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	void UpdateNode(VMMDLL_SCATTER_HANDLE handle);
	void UpdateHealth(VMMDLL_SCATTER_HANDLE handle);
	void UpdateClass(VMMDLL_SCATTER_HANDLE handle);
	uint64_t GetClass() { return ClassAddress; }
	bool GetValid() { return Valid; }
	void SetValid(bool valid) { Valid = valid; }
	std::wstring GetName() { return Names[Type]; };
};
