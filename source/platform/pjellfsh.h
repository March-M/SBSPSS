/*=========================================================================

	pjellfsh.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PJELLFSH_H__
#define __PLATFORM_PLANTERN_H__

#ifndef __PLATFORM_PLINEAR_H__
#include "platform\plinear.h"
#endif

class CNpcJellyfishPlatform : public CNpcLinearPlatform
{
public:
	virtual void		render();
	virtual void		postInit();
	virtual void		think( int _frames );
protected:
	virtual void		collidedWith(CThing *_thisThing);

	s32					m_vertScale;
	u8					m_dipCount;
	s32					m_dipOffset;
};

#endif