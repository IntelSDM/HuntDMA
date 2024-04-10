#pragma once

struct Hitbox
{
	uint64_t Address;
	uint64_t Transform;
	int BodyPart;

};
class PlayerNetworkState
{
private:
	uint64_t Class = 0;
	uint64_t Connected = 0x10; // public bool \u0307\u0307\u0303\u0308\u0305\u0306\u0302\u0308\u0305\u0303\u0301\u0305\u0308\u0306\u0306;
	uint64_t Health = 0x14; // public float \u0302\u0305\u0305\u0308\u0308\u0301\u0303\u0306\u0305\u0303\u0308\u0302\u0306\u0305\u0307;
	uint64_t Friendly = 0xC8;
	uint64_t Hitboxes = 0x110; // 	public \u0306\u0308\u0305\u0305\u0304\u0302\u0304\u0301\u0308\u0304\u0305\u0303\u0305\u0302\u0307[] \u0301\u0306\u0305\u0302\u0308\u0307\u0307\u0305\u0304\u0308\u0305\u0307\u0305\u0304\u0305; 
	// look for an array of a class containing an enum, matrix4x4,transform, boxcollider and another class
	std::vector<Hitbox> HitboxList;
	uint64_t HitboxEnum = 0x38; // public \u0307\u0307\u0302\u0304\u0301\u0301\u0307\u0308\u0308\u0308\u0307\u0306\u0308\u0303\u0301 \u0302\u0308\u0305\u0307\u0306\u0304\u0304\u0303\u0306\u0305\u0307\u0302\u0301\u0308\u0304;
	uint64_t HitboxTransform = 0x58; // 	public Transform \u0307\u0303\u0301\u0305\u0304\u0305\u0307\u0304\u0305\u0306\u0307\u0307\u0305\u0306\u0305;
	uint32_t HitboxSize = 0x18;

	uint64_t PrimaryWeapon = 0x198; // just look for AWeapon in playernetworkstate, top one is primary, second one is secondary
	uint64_t SecondaryWeapon = 0x1A0;
	uint64_t CacheName = 0xD8; //	private string mCacheName;
	uint64_t PrimaryCacheName = 0;
	uint64_t SecondaryCacheName = 0;
	wchar_t PrimaryWeaponName[36] = { '\0' };
	wchar_t SecondaryWeaponName[36] = { '\0' };
	/*
	[Token(Token = "0x40017EE")]
	[FieldOffset(Offset = "0x11C")]
	public Vector3 \u0304\u0306\u0304\u0306\u0301\u0301\u0304\u0302\u0307\u0304\u0306\u0305\u0305\u0306\u0308;

	// Token: 0x040017EF RID: 6127
	[Token(Token = "0x40017EF")]
	[FieldOffset(Offset = "0x128")]
	public Vector3 \u0305\u0301\u0304\u0305\u0303\u0302\u0308\u0302\u0306\u0304\u0303\u0308\u0307\u0306\u0302;

	// Token: 0x040017F0 RID: 6128
	[Token(Token = "0x40017F0")]
	[FieldOffset(Offset = "0x134")]
	public Vector3 \u0307\u0303\u0307\u0304\u0302\u0308\u0305\u0304\u0306\u0305\u0304\u0302\u0306\u0308\u0304;
	Looking for 3 vectors in a row
	*/
	uint64_t PositionOffset = 0x128; // 0x11C

	bool IsConnected = false;
	float CurrentHealth = 0;
	bool IsSeated = false;
	bool IsFriendly = false;
	Vector3 Position;

	void UpdatePrimaryWeapon(VMMDLL_SCATTER_HANDLE handle);
	void UpdateSecondaryWeapon(VMMDLL_SCATTER_HANDLE handle);
	void UpdatePrimaryWeaponName(VMMDLL_SCATTER_HANDLE handle);
	void UpdateSecondaryWeaponName(VMMDLL_SCATTER_HANDLE handle);
	void UpdatePrimaryCacheName(VMMDLL_SCATTER_HANDLE handle);
	void UpdateSecondaryCacheName(VMMDLL_SCATTER_HANDLE handle);
	void UpdateHitboxes(VMMDLL_SCATTER_HANDLE handle);



public:
	PlayerNetworkState(uint64_t address, VMMDLL_SCATTER_HANDLE handle);
	void UpdateConnected(VMMDLL_SCATTER_HANDLE handle );
	void UpdateHealth(VMMDLL_SCATTER_HANDLE handle);
	void UpdateFriendly(VMMDLL_SCATTER_HANDLE handle);
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);

	void UpdateWeapons(VMMDLL_SCATTER_HANDLE handle);
	void UpdateWeapons1(VMMDLL_SCATTER_HANDLE handle);

	float GetHealth();
	bool GetConnected();
	bool GetFriendly();
	Vector3 GetPosition();

	std::wstring GetPrimaryWeaponName();
	std::wstring GetSecondaryWeaponName();

};