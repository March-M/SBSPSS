/*=========================================================================

	pspatula.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pspatula.h"

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::init()
{
	CBasePickup::init();
	m_glint=0;
	m_glintRot=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CSpatulaPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=getSpriteBank()->getFrameHeader(FRM__SPATULA);
	size.vx=fh->W;
	size.vy=fh->H;
	return size;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int spat_glintspeed=1;
int spat_maxglint=100;
int spat_glintrotspeed=50;
int spat_glintgrowspeed=5;
DVECTOR	spat_gxy1={0,28};
DVECTOR spat_gxy2={50,-35};
static const int spat_glintFrames[]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4,FRM__GLINT4,FRM__GLINT3,FRM__GLINT2,FRM__GLINT1};
void	CSpatulaPickup::thinkPickup(int _frames)
{
	m_glint+=_frames*spat_glintspeed;
	if(m_glint>spat_maxglint)m_glint-=spat_maxglint;
	m_glintRot+=_frames*spat_glintrotspeed;
	m_glintRot&=4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__SPATULA);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);

	if(m_glint<=255)
	{
		fh=sprites->getFrameHeader(spat_glintFrames[(m_glint>>spat_glintgrowspeed)&0x07]);
		x=x+(((spat_gxy2.vx-spat_gxy1.vx)*m_glint)>>8)+spat_gxy1.vx;
		y=y+(((spat_gxy2.vy-spat_gxy1.vy)*m_glint)>>8)+spat_gxy1.vy;
		sprites->printRotatedScaledSprite(fh,x,y,4095,4095,m_glintRot,OTPOS__PICKUP_POS-1);
	}
}




int bspat_height=30;
int bspat_r1=251;
int bspat_g1=207;
int bspat_b1=167;
int bspat_r2=147;
int bspat_g2=95;
int bspat_b2=75;
int bspat_speed=25;
int bspat_scale1=5;
int bspat_scale2=0;//2;
int bspat_phase=1024;
int bspat_vissize=40;
int	bspat_stringx=-3;
int bspat_stringendxoff=3;
int bspat_stringendyoff=-6;
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonAndSpatulaPickup::init()
{
	CSpatulaPickup::init();
	m_sin=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonAndSpatulaPickup::thinkPickup(int _frames)
{
	CSpatulaPickup::thinkPickup(_frames);
	m_sin=(m_sin+(_frames*bspat_speed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonAndSpatulaPickup::renderPickup(DVECTOR *_pos)
{
	CSpatulaPickup::renderPickup(_pos);

	SpriteBank	*sprites;
	sFrameHdr	*fh,*fhspat;
	int			xo1,xo2;
	int			x,y;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BALLOON);
	fhspat=sprites->getFrameHeader(FRM__SPATULA);

	xo1=((msin((m_sin+bspat_phase)&4095)*bspat_scale1)>>12);
	xo2=((msin(m_sin)*bspat_scale2)>>12);
	x=_pos->vx-(fh->W/2)+bspat_stringendxoff;
	y=_pos->vy-(fh->H/2)-(fhspat->H/2)-bspat_height+bspat_stringendyoff;
	sprites->printFT4(fh,x+xo1,y,0,0,OTPOS__PICKUP_POS);
//setCollisionCentreOffset(xo1,0);

	x=_pos->vx+bspat_stringx+bspat_stringendxoff;
	y=_pos->vy+(fh->H/2)-(fhspat->H/2)-bspat_height+bspat_stringendyoff;
	DrawLine(x+xo1,y,x+xo2,y+bspat_height,bspat_r1,bspat_g1,bspat_b1,OTPOS__PICKUP_POS);
	x++;
	DrawLine(x+xo1,y,x+xo2,y+bspat_height,bspat_r2,bspat_g2,bspat_b2,OTPOS__PICKUP_POS);
}

/*===========================================================================
end */
