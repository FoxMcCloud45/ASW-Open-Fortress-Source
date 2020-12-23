// ========= Copyright Open Fortress Developers, CC-BY-NC-SA ============
// Purpose: Root Panel of VGUI elements
// Author(s): ficool2
//

#ifndef OF_ROOTPANEL_H
#define OF_ROOTPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>
#include <vgui_controls/EditablePanel.h>
#include "utlvector.h"

class CPanelEffect;

// Serial under of effect, for safe lookup
typedef unsigned int EFFECT_HANDLE;

class C_OFRootPanel : public vgui::Panel
{
	typedef vgui::Panel BaseClass;
	
public:
						C_OFRootPanel( vgui::VPANEL parent, int slot );
	virtual				~C_OFRootPanel( void );

	// Draw Panel effects here
	virtual void		PostChildPaint();

	// Clear list of Panel Effects
	virtual void		LevelInit( void );
	virtual void		LevelShutdown( void );

	// Run effects and let them decide whether to remove themselves
	void				OnTick( void );

	virtual void		PaintTraverse( bool Repaint, bool allowForce = true );

	virtual void		OnThink();

private:
	// Render all panel effects
	void		RenderPanelEffects( void );

	// List of current panel effects
	CUtlVector< CPanelEffect *> m_Effects;
	int			m_nSplitSlot;
};


#endif