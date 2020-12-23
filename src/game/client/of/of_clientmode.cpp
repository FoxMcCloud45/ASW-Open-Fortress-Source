// ========= Copyright Open Fortress Developers, CC-BY-NC-SA ============
// Purpose: Clientside Manager
// Author(s): ficool2
//

#include "cbase.h"
#include "hud.h"
#include "of_clientmode.h"
#include "cdll_client_int.h"
#include "iinput.h"
#include "ivmodemanager.h"
#include "ivrenderview.h"
#include "panelmetaclassmgr.h"
#include "glow_outline_effect.h"
#include "view_shared.h"
#include "view.h"
#include "viewpostprocess.h"

#define SCREEN_FILE		"scripts/vgui_screens.txt"

ConVar default_fov( "default_fov", "90", FCVAR_CHEAT );

// IClientMode *g_pClientMode = NULL;

static IClientMode *g_pClientMode[ MAX_SPLITSCREEN_PLAYERS ];
IClientMode *GetClientMode()
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	return g_pClientMode[ GET_ACTIVE_SPLITSCREEN_SLOT() ];
}

class COFModeManager : public IVModeManager
{
public:
	virtual void	Init();
	virtual void	SwitchMode( bool commander, bool force ) {}
	virtual void	LevelInit( const char *newmap );
	virtual void	LevelShutdown( void );
	virtual void	ActivateMouse( bool isactive ) {}
};

static COFModeManager g_ModeManager;
IVModeManager *modemanager = ( IVModeManager * )&g_ModeManager;

void COFModeManager::Init()
{
	// g_pClientMode = GetClientModeNormal();
	
	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		g_pClientMode[ i ] = GetClientModeNormal();
	}

	
	PanelMetaClassMgr()->LoadMetaClassDefinitionFile( SCREEN_FILE );
}

void COFModeManager::LevelInit( const char *newmap )
{
	//g_pClientMode->LevelInit( newmap );

	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		g_pClientMode[ i ]->LevelInit( newmap );
	}
}

void COFModeManager::LevelShutdown( void )
{
	//g_pClientMode->LevelShutdown();

	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		GetClientMode()->LevelShutdown();
	}
}

ClientModeOFNormal::ClientModeOFNormal()
{
}

ClientModeOFNormal::~ClientModeOFNormal()
{
}

void ClientModeOFNormal::InitViewport()
{
	m_pViewport = new OFViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}

ClientModeOFNormal g_ClientModeNormal[ MAX_SPLITSCREEN_PLAYERS ];

IClientMode *GetClientModeNormal()
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	return &g_ClientModeNormal[ GET_ACTIVE_SPLITSCREEN_SLOT() ];
}

ClientModeOFNormal* GetClientModeOFNormal()
{
	return static_cast< ClientModeOFNormal* >( GetClientModeNormal() );
}

class FullscreenOFViewport : public OFViewport
{
private:
	DECLARE_CLASS_SIMPLE( FullscreenOFViewport, OFViewport );

private:
	virtual void InitViewportSingletons( void )
	{
		SetAsFullscreenViewportInterface();
	}
};

class ClientModeOFNormalFullscreen : public	ClientModeOFNormal
{
	DECLARE_CLASS_SIMPLE( ClientModeOFNormalFullscreen, ClientModeOFNormal );
public:
	virtual void InitViewport()
	{
		// Skip over BaseClass!!!
		BaseClass::BaseClass::InitViewport();
		m_pViewport = new FullscreenOFViewport();
		m_pViewport->Start( gameuifuncs, gameeventmanager );
	}
};

//--------------------------------------------------------------------------------------------------------
ClientModeOFNormalFullscreen g_FullscreenClientMode;
IClientMode *GetFullscreenClientMode( void )
{
	return &g_FullscreenClientMode;
}

float ClientModeOFNormal::GetViewModelFOV( void )
{
	//Tony; retrieve the fov from the view model script, if it overrides it.
	float viewFov = 74.0;

	//C_WeaponSDKBase *pWeapon = (C_WeaponSDKBase*)GetActiveWeapon();
	//if ( pWeapon )
	//{
	//	viewFov = pWeapon->GetWeaponFOV();
	//}
	return viewFov;
}

int ClientModeOFNormal::GetDeathMessageStartHeight( void )
{
	return m_pViewport->GetDeathMessageStartHeight();
}

void ClientModeOFNormal::PostRenderVGui()
{
}

bool ClientModeOFNormal::CanRecordDemo( char *errorMsg, int length ) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSetup - 
//-----------------------------------------------------------------------------
void ClientModeOFNormal::OverrideView( CViewSetup *pSetup )
{
	QAngle camAngles;

	// Let the player override the view.
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if(!pPlayer)
		return;

	pPlayer->OverrideView( pSetup );

	if( ::input->CAM_IsThirdPerson() )
	{
		Vector cam_ofs;

		::input->CAM_GetCameraOffset( cam_ofs );

		camAngles[ PITCH ] = cam_ofs[ PITCH ];
		camAngles[ YAW ] = cam_ofs[ YAW ];
		camAngles[ ROLL ] = 0;

		Vector camForward, camRight, camUp;
		AngleVectors( camAngles, &camForward, &camRight, &camUp );

		VectorMA( pSetup->origin, -cam_ofs[ ROLL ], camForward, pSetup->origin );

		static ConVarRef c_thirdpersonshoulder( "c_thirdpersonshoulder" );
		if ( c_thirdpersonshoulder.GetBool() )
		{
			static ConVarRef c_thirdpersonshoulderoffset( "c_thirdpersonshoulderoffset" );
			static ConVarRef c_thirdpersonshoulderheight( "c_thirdpersonshoulderheight" );
			static ConVarRef c_thirdpersonshoulderaimdist( "c_thirdpersonshoulderaimdist" );

			// add the shoulder offset to the origin in the cameras right vector
			VectorMA( pSetup->origin, c_thirdpersonshoulderoffset.GetFloat(), camRight, pSetup->origin );

			// add the shoulder height to the origin in the cameras up vector
			VectorMA( pSetup->origin, c_thirdpersonshoulderheight.GetFloat(), camUp, pSetup->origin );

			// adjust the yaw to the aim-point
			camAngles[ YAW ] += RAD2DEG( atan(c_thirdpersonshoulderoffset.GetFloat() / (c_thirdpersonshoulderaimdist.GetFloat() + cam_ofs[ ROLL ])) );

			// adjust the pitch to the aim-point
			camAngles[ PITCH ] += RAD2DEG( atan(c_thirdpersonshoulderheight.GetFloat() / (c_thirdpersonshoulderaimdist.GetFloat() + cam_ofs[ ROLL ])) );
		}

		// Override angles from third person camera
		VectorCopy( camAngles, pSetup->angles );
	}
	else if (::input->CAM_IsOrthographic())
	{
		pSetup->m_bOrtho = true;
		float w, h;
		::input->CAM_OrthographicSize( w, h );
		w *= 0.5f;
		h *= 0.5f;
		pSetup->m_OrthoLeft   = -w;
		pSetup->m_OrthoTop    = -h;
		pSetup->m_OrthoRight  = w;
		pSetup->m_OrthoBottom = h;
	}
}

void ClientModeOFNormal::DoPostScreenSpaceEffects( const CViewSetup *pSetup )
{
	//extern bool g_bRenderingGlows;
	
	// Render object glows and selectively-bloomed objects
	//g_bRenderingGlows = true;
	g_GlowObjectManager.RenderGlowEffects( pSetup, GetSplitScreenPlayerSlot() );
	//g_bRenderingGlows = false;
}

void ClientModeOFNormal::Update( void )
{
}

void ClientModeOFNormal::Shutdown()
{
}

void ClientModeOFNormal::LevelInit( const char *newmap )
{
	BaseClass::LevelInit(newmap);
}

