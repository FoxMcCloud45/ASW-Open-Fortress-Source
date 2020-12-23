// ========= Copyright Open Fortress Developers, CC-BY-NC-SA ============
// Purpose: Clientside Manager
// Author(s): ficool2
//

#ifndef OF_CLIENTMODE_H
#define OF_CLIENTMODE_H
#ifdef _WIN32
#pragma once
#endif

#include "clientmode_shared.h"
#include "vgui/of_viewport.h"

class ClientModeOFNormal : public ClientModeShared 
{
	DECLARE_CLASS( ClientModeOFNormal, ClientModeShared );
	
public:
					ClientModeOFNormal();
	virtual			~ClientModeOFNormal();
	virtual void	InitViewport();
	virtual float	GetViewModelFOV( void );
	int				GetDeathMessageStartHeight( void );
	virtual void	PostRenderVGui();
	void			DoPostScreenSpaceEffects( const CViewSetup *pSetup );
	virtual bool	CanRecordDemo( char *errorMsg, int length ) const;
	virtual void	OverrideView( CViewSetup *pSetup );
	virtual void	Update( void );
	virtual void	Shutdown();
	virtual void	LevelInit( const char *newmap );

};


extern IClientMode *GetClientModeNormal();
extern ClientModeOFNormal* GetClientModeOFNormal();

#endif
