//========= Copyright 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

// FoxMcCloud45 Modifications (CC-BY-NC-CA)
// * added check for OF_CLIENT_DLL define, based on Open Fortress Modifications.
// * adapted crosshair's color drawing to the Alien Swarm code.

#include "cbase.h"
#include "hud.h"
#include "hud_crosshair.h"
#include "iclientmode.h"
#include "view.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include "IVRenderView.h"



// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar crosshair( "crosshair", "1", FCVAR_ARCHIVE );
ConVar cl_observercrosshair( "cl_observercrosshair", "1", FCVAR_ARCHIVE );

using namespace vgui;

int ScreenTransform( const Vector& point, Vector& screen );

DECLARE_HUDELEMENT( CHudCrosshair );

CHudCrosshair::CHudCrosshair( const char *pElementName ) :
  CHudElement( pElementName ), BaseClass( NULL, "HudCrosshair" )
{
	vgui::Panel *pParent = GetClientMode()->GetViewport();
	SetParent( pParent );

	m_pCrosshair = 0;

	m_clrCrosshair = Color( 0, 0, 0, 0 );

	m_vecCrossHairOffsetAngle.Init();

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_CROSSHAIR );
}

void CHudCrosshair::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	m_pDefaultCrosshair = HudIcons().GetIcon("crosshair_default");
	SetPaintBackgroundEnabled( false );

    SetSize( ScreenWidth(), ScreenHeight() );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudCrosshair::ShouldDraw( void )
{
	bool bNeedsDraw;

	if ( m_bHideCrosshair )
		return false;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return false;

	C_BaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	if ( pWeapon && !pWeapon->ShouldDrawCrosshair() )
		return false;

	/* disabled to avoid assuming it's an HL2 player.
	// suppress crosshair in zoom.
	if ( pPlayer->m_HL2Local.m_bZooming )
		return false;
	*/

	// draw a crosshair only if alive or spectating in eye
	if ( IsX360() )
	{
		bNeedsDraw = m_pCrosshair && 
			!engine->IsDrawingLoadingImage() &&
			!engine->IsPaused() && 
			( !pPlayer->IsSuitEquipped() || g_pGameRules->IsMultiplayer() ) &&
			GetClientMode()->ShouldDrawCrosshair() &&
			!( pPlayer->GetFlags() & FL_FROZEN ) &&
			( pPlayer->IsViewEntity() ) &&
			( pPlayer->IsAlive() ||	( pPlayer->GetObserverMode() == OBS_MODE_IN_EYE ) || ( cl_observercrosshair.GetBool() && pPlayer->GetObserverMode() == OBS_MODE_ROAMING ) );
	}
	else
	{
		bNeedsDraw = m_pCrosshair && 
			crosshair.GetInt() &&
			!engine->IsDrawingLoadingImage() &&
			!engine->IsPaused() && 
			GetClientMode()->ShouldDrawCrosshair() &&
			!( pPlayer->GetFlags() & FL_FROZEN ) &&
			( pPlayer->IsViewEntity() ) &&
			!pPlayer->IsInVGuiInputMode() &&
			( pPlayer->IsAlive() ||	( pPlayer->GetObserverMode() == OBS_MODE_IN_EYE ) || ( cl_observercrosshair.GetBool() && pPlayer->GetObserverMode() == OBS_MODE_ROAMING ) );
	}

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

#if defined( OF_CLIENT_DLL )
// Crosshair RGB values & scale.
ConVar cl_crosshair_red	 ( "cl_crosshair_red", "255", FCVAR_ARCHIVE, "Value for the crosshair's Red color (RGB)." );
ConVar cl_crosshair_green( "cl_crosshair_green", "255", FCVAR_ARCHIVE, "Value for the crosshair's Green color (RGB)." );
ConVar cl_crosshair_blue ( "cl_crosshair_blue", "255", FCVAR_ARCHIVE, "Value for the crosshair's Blue color (RGB).");
// TODO: IMPLEMENT SCALE FOR OPEN FORTRESS.
// ConVar cl_crosshair_scale( "cl_crosshair_scale", "1",   FCVAR_ARCHIVE, "Scale for the crosshair." );
#endif

void CHudCrosshair::Paint( void )
{
	if ( !m_pCrosshair )
		return;

	if ( !IsCurrentViewAccessAllowed() )
		return;

	float x, y;
	x = ScreenWidth()/2;
	y = ScreenHeight()/2;

	m_curViewAngles = CurrentViewAngles();
	m_curViewOrigin = CurrentViewOrigin();

	Vector screen;
	screen.Init();

	// TrackIR
	if ( IsHeadTrackingEnabled() )
	{
		C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
		if ( !pPlayer )
			return;

		// TrackIR
		// get the direction the player is aiming
		Vector aimVector = pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

		// calculate where the bullet would go so we can draw the cross appropriately
		Vector vecEnd = pPlayer->Weapon_ShootPosition() + aimVector * MAX_TRACE_LENGTH;

		trace_t tr;
		UTIL_TraceLine( pPlayer->Weapon_ShootPosition(), vecEnd, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );

		QAngle angles;
		Vector forward;
		Vector point;

		// this code is wrong
		angles = m_curViewAngles + m_vecCrossHairOffsetAngle;
		AngleVectors( angles, &forward );

		// need to project forward into an object to see how far this 
		// vector should be!!
		//forward *= 1000;

		//VectorAdd( m_curViewOrigin, forward, point );
		//ScreenTransform( point, screen );

		ScreenTransform(tr.endpos, screen);
	}
	// TrackIR
	else
	{
		// MattB - m_vecCrossHairOffsetAngle is the autoaim angle.
		// if we're not using autoaim, just draw in the middle of the 
		// screen
		if ( m_vecCrossHairOffsetAngle != vec3_angle )
		{
			QAngle angles;
			Vector forward;
			Vector point;

			// this code is wrong
			angles = m_curViewAngles + m_vecCrossHairOffsetAngle;
			AngleVectors( angles, &forward );
			VectorAdd( m_curViewOrigin, forward, point );
			ScreenTransform( point, screen );
		}
	}

	//x += 0.5f * screen[0] * ScreenWidth() + 0.5f;
	//y += 0.5f * screen[1] * ScreenHeight() + 0.5f;


	int width = MAX( 1, y * 0.03f );
	int height = MAX( 1, y * 0.005f );

#if defined( OF_CLIENT_DLL )
	surface()->DrawSetColor( cl_crosshair_red.GetInt(), cl_crosshair_green.GetInt(), cl_crosshair_blue.GetInt(), 255 );
#else
	surface()->DrawSetColor( 128, 196, 220, 255 );
#endif

	// TODO: IMPLEMENT SCALE FOR OPEN FORTRESS.
	surface()->DrawFilledRect( x - width - height,
		y - height / 2,
		x - width,
		y + height / 2 );

	surface()->DrawFilledRect( x + width,
		y - height / 2,
		x + width + height,
		y + height / 2 );

	surface()->DrawFilledRect( x - height / 2,
		y - width - height,
		x + height / 2,
		y - width );

	surface()->DrawFilledRect( x - height / 2,
		y + width,
		x + height / 2,
		y + width + height );

	surface()->DrawFilledRect( x - height / 2,
		y - height / 2,
		x + height / 2,
		y + height / 2 );


	//m_pCrosshair->DrawSelf( 
	//		x - 0.5f * m_pCrosshair->Width(), 
	//		y - 0.5f * m_pCrosshair->Height(),
	//		m_clrCrosshair );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCrosshair::SetCrosshairAngle( const QAngle& angle )
{
	VectorCopy( angle, m_vecCrossHairOffsetAngle );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCrosshair::SetCrosshair( CHudTexture *texture, const Color& clr )
{
	m_pCrosshair = texture;
	m_clrCrosshair = clr;
}

//-----------------------------------------------------------------------------
// Purpose: Resets the crosshair back to the default
//-----------------------------------------------------------------------------
void CHudCrosshair::ResetCrosshair()
{
	SetCrosshair( m_pDefaultCrosshair, Color(255, 255, 255, 255) );
}
