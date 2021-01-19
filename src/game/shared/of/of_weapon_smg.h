// ========= Copyright Open Fortress Developers, CC-BY-NC-SA ============
// Purpose: 
// Author(s): KaydemonLP
//

// FoxMcCloud45 modifications
// * OFWeaponID to int
#pragma once

#include "of_weapon_base_gun.h"

// Client specific.
#ifdef CLIENT_DLL
#define COFSMG C_OFSMG
#define COFShotgun C_OFShotgun
#endif



//=============================================================================
//
// TF Weapon Sub-machine gun.
//
class COFSMG : public COFWeaponBaseGun
{
public:

	DECLARE_CLASS( COFSMG, COFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif

	virtual int	GetWeaponID(void) const			{ return OF_WEAPON_SMG; }
};

class COFShotgun : public COFWeaponBaseGun
{
public:

	COFShotgun();

	DECLARE_CLASS( COFShotgun, COFWeaponBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif

	virtual int	GetWeaponID(void) const			{ return OF_WEAPON_SHOTGUN; }
};