/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/
/*
Sprite/object control - Thrown objects, so need curved path, :o(
FX
1	Sandwich
2	makeup bag
3	uniform
4	kelp bar
5	fix tv

*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fma\fma.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __BACKEND_PARTY_H__
#include "backend\party.h"
#endif

#ifndef __THING_THING_H__
#include "thing\thing.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#include	"game\game.h"
#include	"utils\utils.h"
#include	"pad\pads.h"
#include	"gfx\font.h"
#include	"gfx\otpos.h"
#include	<sprites.h>

#include	"fx\fx.h"
#include	"fx\FXTvExplode.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

// Game scenes..
#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef __FRONTEND_CREDITS_H__
#include "backend\credits.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif



/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include "actor_spongebob_anim.h"
#endif
#include "actor_spongebob_fma_anim.h"

#ifndef	__ANIM_MERMAIDMAN_HEADER__
#include "actor_mermaidman_anim.h"
#endif

#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include "actor_barnacleboy_anim.h"
#endif

#ifndef	__ANIM_GARY_HEADER__
#include "actor_gary_anim.h"
#endif

#ifndef	__ANIM_PLANKTON_HEADER__
#include "actor_plankton_anim.h"
#endif

#ifndef	__ANIM_PATRICK_HEADER__
#include "actor_patrick_anim.h"
#endif

#ifndef	__ANIM_KRUSTY_HEADER__
#include "actor_krusty_anim.h"
#endif

#ifndef	__ANIM_squidward_HEADER__
#include "actor_squidward_anim.h"
#endif

#ifndef	__ANIM_SANDY_HEADER__
#include "actor_sandy_anim.h"
#endif

#define	FRM__KELP_BAR	FRM__C4_QUEST_ITEM_2

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Available actors
enum
{
	FMA_ACTOR_SPONGEBOB,
	FMA_ACTOR_BB,
	FMA_ACTOR_MM,
	FMA_ACTOR_GARY,
	FMA_ACTOR_PLANKTON,
	FMA_ACTOR_PATRICK,
	FMA_ACTOR_KRABS,
	FMA_ACTOR_SQUIDWARD,
	FMA_ACTOR_SANDY,

	FMA_NUM_ACTORS
};

// Actor animation types
enum	
{
	FMA_ANIM_IDLE,
	FMA_ANIM_WALK,
	
	FMA_ANIM_FIXTV,
	FMA_ANIM_GIVEEND,
	FMA_ANIM_GIVESTART,
	FMA_ANIM_IDEA,
	FMA_ANIM_QUICKEXIT,
	FMA_ANIM_SHOUT,
	FMA_ANIM_STUMBLE,
	FMA_ANIM_THROW,

	FMA_ANIM_HIDE,
	FMA_ANIM_UNHIDE,
	FMA_ANIM_UNHIDEIDLE,
	FMA_ANIM_SIT,
	FMA_ANIM_SITLOOKLEFT,
	FMA_ANIM_SITASLEEP,
	FMA_ANIM_RUBHEAD,

	FMA_NUM_ANIMS,
};

// Next scene types
enum
{
	FMA_NEXT_SCENE_MAP,
	FMA_NEXT_SCENE_GAME,
	FMA_NEXT_SCENE_CREDITS,

	FMA_NUM_NEXT_SCENES
};


// Available script commands
typedef enum
{
	SC_USE_LEVEL,				// levelNumber
	SC_USE_PARTY,				// 
	SC_SET_NEXT_SCENE,			// nextScene

	SC_SNAP_CAMERA_TO,			// x,y
	SC_MOVE_CAMERA_TO,			// x,y,frames

	SC_REGISTER_CONVERSATION,	// scriptId

	SC_WAIT_ON_TIMER,			// frames						(nonblocking)
	SC_WAIT_ON_ACTOR_STOP,		// actor						(nonblocking)
	SC_WAIT_ON_ACTOR_ANIM,		// actor						(nonblocking)
	SC_WAIT_ON_CAMERA_STOP,		//								(nonblocking)
	SC_WAIT_ON_CONVERSATION,	// scriptId						(nonblocking)
	SC_WAIT_ON_THROWN_ITEM,		// item
	
	SC_SET_ACTOR_VISIBILITY,	// actor,on/off
	SC_SET_ACTOR_POSITION,		// actor,x,y
	SC_SET_ACTOR_FACING,		// actor,facing
	SC_SET_ACTOR_ANIM_STATE,	// actor,state,loop
	SC_WALK_ACTOR_TO_POSITION,	// actor,x,y,frames
	SC_SET_GLOBAL_ANIMATION,	// on/off

	SC_CREATE_FX,				// FxNo, X,Y, FXType
	SC_KILL_FX,					// FxNo

	SC_SET_ITEM,				// item,frame
	SC_CARRY_ITEM,				// item,actor
	SC_HIDE_ITEM,				// item
	SC_THROW_ITEM_TO_ACTOR,		// item,targetActor,arcHeight,frames

	SC_START,					// 
	SC_STOP,					// 
} SCRIPT_COMMAND;

/*
struct	sFMAItemTable
{
		u16		Frame;
		DVECTOR	SprOfs;
};

enum	FMA_ITEM
{
	FMA_ITEM_C1,
	FMA_ITEM_C2,
	FMA_ITEM_C3,
	FMA_ITEM_C4,
	FMA_ITEM_C5,
	FMA_ITEM_MAX
}

sFMAItemTable	FMAItemTable[FMA_ITEM_MAX]
{
};
*/
/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Data for an actors graphics
struct	sFMAAnim
{
		s16	Bank;
		s16	Anim;
};
typedef struct
{
	FileEquate		m_file[2];
	DVECTOR			m_ItemOfs;
	sFMAAnim		m_anims[FMA_NUM_ANIMS];

} ACTOR_GRAPHICS_DATA;

// Actor data
typedef struct
{
	CActorGfx	*m_gfx[2];
	u8			m_active;

	s8			m_animState,m_animFrame;
	s8			m_animLoop;
	
	DVECTOR		m_pos;
	
	u16			m_startMoveFrame;
	DVECTOR		m_startMovePos;
	u16			m_endMoveFrame;
	DVECTOR		m_endMovePos;
	u8			m_moving;

	u8			m_facing;
} ACTOR_DATA;

struct	sItemData
{
		s8			m_visible;

		DVECTOR		m_Pos;
		s8			m_Actor;
		u8			m_facing;
		u16			m_Frame;

		s8			m_TargetActor;
		DVECTOR		m_startPos;
		DVECTOR		m_TargetPos;
		u16			m_startMoveFrame;
		u16			m_endMoveFrame;
		s8			m_throwArcHeight;
};


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */
enum
{
	FMA_FX_COUNT=4,
	FMA_ITEM_MAX=4,
};

CFmaScene	FmaScene;
CFX			*m_FXTable[FMA_FX_COUNT];
sItemData	m_itemData[FMA_ITEM_MAX];

/*****************************************************************************/
static CScene	*s_nextGameSceneTable[FMA_NUM_NEXT_SCENES]=
{
	&MapScene,					// FMA_NEXT_SCENE_MAP
	&GameScene,					// FMA_NEXT_SCENE_GAME
	&CreditsScene,				// FMA_NEXT_SCENE_CREDITS
};


/*****************************************************************************/
// Actor graphics data
static const ACTOR_GRAPHICS_DATA	s_actorGraphicsData[FMA_NUM_ACTORS]=
{
	{ // SpongeBob
		{ACTORS_SPONGEBOB_SBK,ACTORS_SPONGEBOB_FMA_SBK},{-48,-48},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_SPONGEBOB_IDLEBREATH},
/*FMA_ANIM_WALK*/				{0,ANIM_SPONGEBOB_RUN},
/*FMA_ANIM_FIXTV*/				{1,ANIM_SPONGEBOB_FMA_FIXTV},
/*FMA_ANIM_GIVEEND*/			{1,ANIM_SPONGEBOB_FMA_GIVEEND},
/*FMA_ANIM_GIVESTART*/			{1,ANIM_SPONGEBOB_FMA_GIVESTART},
/*FMA_ANIM_IDEA*/				{1,ANIM_SPONGEBOB_FMA_IDEA},
/*FMA_ANIM_QUICKEXIT*/			{1,ANIM_SPONGEBOB_FMA_QUICKEXIT},
/*FMA_ANIM_SHOUT*/				{1,ANIM_SPONGEBOB_FMA_SHOUT},
/*FMA_ANIM_STUMBLE*/			{1,ANIM_SPONGEBOB_FMA_STUMBLE},
/*FMA_ANIM_THROW*/				{1,ANIM_SPONGEBOB_FMA_THROW},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Barnicle Boy
		{ACTORS_BARNACLEBOY_SBK,	(FileEquate)0},{-32,-48},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_BARNACLEBOY_IDLE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,ANIM_BARNACLEBOY_GIVEEND},
/*FMA_ANIM_GIVESTART*/			{0,ANIM_BARNACLEBOY_GIVESTART},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,ANIM_BARNACLEBOY_HIDE},
/*FMA_ANIM_UNHIDE*/				{0,ANIM_BARNACLEBOY_UNHIDE},
/*FMA_ANIM_UNHIDEIDLE*/			{0,ANIM_BARNACLEBOY_UNHIDEIDLE},
/*FMA_ANIM_SIT*/				{0,ANIM_BARNACLEBOY_SIT},
/*FMA_ANIM_SITLOOKLEFT*/		{0,ANIM_BARNACLEBOY_SITLOOKLEFT},
/*FMA_ANIM_SITASLEEP*/			{0,ANIM_BARNACLEBOY_SITASLEEP},
/*FMA_ANIM_RUBHEAD*/			{0,ANIM_BARNACLEBOY_RUBHEAD},
		},
	},
	{ // Mermaid Man
		{ACTORS_MERMAIDMAN_SBK,	(FileEquate)0},{-32,-64},	
		{	
/*FMA_ANIM_IDLE*/				{0,ANIM_MERMAIDMAN_IDLE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,ANIM_MERMAIDMAN_HIDE},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,ANIM_MERMAIDMAN_SIT},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,ANIM_MERMAIDMAN_SITASLEEP},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Gary Da Snail
		{ACTORS_GARY_SBK,		(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_GARY_IDLE},
/*FMA_ANIM_WALK*/				{0,ANIM_GARY_IDLE},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Plankton
		{ACTORS_PLANKTON_SBK,		(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_PLANKTON_IDLE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Patrick
		{ACTORS_PATRICK_SBK,		(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_PATRICK_IDLEBREATHE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Krusty
		{ACTORS_KRUSTY_SBK,			(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_KRUSTY_IDLEBREATHE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Squidward
		{ACTORS_SQUIDWARD_SBK,		(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_SQUIDWARD_IDLEBREATHE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
	{ // Sandy
		{ACTORS_SANDY_SBK,			(FileEquate)0},{0,0},	
		{
/*FMA_ANIM_IDLE*/				{0,ANIM_SANDY_IDLE},
/*FMA_ANIM_WALK*/				{0,-1},
/*FMA_ANIM_FIXTV*/				{0,-1},
/*FMA_ANIM_GIVEEND*/			{0,-1},
/*FMA_ANIM_GIVESTART*/			{0,-1},
/*FMA_ANIM_IDEA*/				{0,-1},
/*FMA_ANIM_QUICKEXIT*/			{0,-1},
/*FMA_ANIM_SHOUT*/				{0,-1},
/*FMA_ANIM_STUMBLE*/			{0,-1},
/*FMA_ANIM_THROW*/				{0,-1},
/*FMA_ANIM_HIDE*/				{0,-1},
/*FMA_ANIM_UNHIDE*/				{0,-1},
/*FMA_ANIM_UNHIDEIDLE*/			{0,-1},
/*FMA_ANIM_SIT*/				{0,-1},
/*FMA_ANIM_SITLOOKLEFT*/		{0,-1},
/*FMA_ANIM_SITASLEEP*/			{0,-1},
/*FMA_ANIM_RUBHEAD*/			{0,-1},
		},
	},
};


ACTOR_DATA		m_actorData[FMA_NUM_ACTORS];


/*****************************************************************************/
/*** Intro FMA ***************************************************************/
/*****************************************************************************/
static const int s_FMAIntroScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_03_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,


// Scene 1 - SB & Gary outside house
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,80*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_GARY,84*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_GARY,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_GARY,true,

	SC_SNAP_CAMERA_TO,			67*16,18*16,

	SC_WAIT_ON_TIMER,			60,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_00_DAT,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,60*16,30*16,60*2,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,64*16,30*16,60*3,

	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
//	SC_WAIT_ON_TIMER,			60*2,

// Scene 2 - Shady shoals with BB & MM
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,11*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,60*16,30*16,60*2,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,17*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 3 - SB Arives
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,24*16,30*16,60*2,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_GARY,44*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,28*16,30*16,60*3,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_GARY,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_IDLE,1,

// Scene 4 - SB talks to em
// Scene 5 - MM gives task
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_01_DAT,

// Scene 7 - SB Leaves
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_02_DAT,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_RUBHEAD,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,40*16,30*16,30,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 8 - Back to Gary, for non funny end
	SC_SET_ACTOR_FACING,		FMA_ACTOR_GARY,0,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,30*16,30*16,60,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_GARY,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_03_DAT,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,44*16,30*16,60*2,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_GARY,

	SC_STOP
};

/*****************************************************************************/
/*** C1 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC1EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_03_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			60*2,

// Scene 2 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_SITASLEEP,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,208*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SITASLEEP,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,211*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,
	SC_CREATE_FX,				0,207*16,(26*16),CFX::FX_TYPE_ZZZZ,
	SC_CREATE_FX,				1,211*16,(26*16),CFX::FX_TYPE_ZZZZ,

	SC_SNAP_CAMERA_TO,			197*16,18*16,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_00_DAT,
	SC_WAIT_ON_TIMER,			30,

// Scene 3 - SB enter by door
	SC_MOVE_CAMERA_TO,			235*16,18*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_SET_ITEM,				0,FRM__SANDWICH,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,230*16,30*16,120,
	SC_MOVE_CAMERA_TO,			210*16,18*16,120,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_STUMBLE,0,
	SC_THROW_ITEM_TO_ACTOR,		0,FMA_ACTOR_MM,48,120,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_01_DAT,

// scene 6 - sarnie flys thru the air
//	SC_SPRITE_SHIT
	SC_MOVE_CAMERA_TO,			197*16,18*16,120,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,216*16,30*16,56,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_THROWN_ITEM,		0,
	SC_CARRY_ITEM,				0,FMA_ACTOR_MM,

// Scene 7 - MM Eats sarnie
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_KILL_FX,					1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SITLOOKLEFT,1,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_02_DAT,


// Scene 8 - SB Leaves
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_03_DAT,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
//	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_RUBHEAD,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_STOP
};

/*****************************************************************************/
/*** C2 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC2EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_02_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			60*2,

// Scene 2 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_SITASLEEP,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,208*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SITASLEEP,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,211*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,
	SC_CREATE_FX,				0,207*16,(26*16),CFX::FX_TYPE_ZZZZ,
	SC_CREATE_FX,				1,211*16,(26*16),CFX::FX_TYPE_ZZZZ,

	SC_SNAP_CAMERA_TO,			197*16,18*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 3 - SB enter by door
	SC_MOVE_CAMERA_TO,			235*16,18*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_SET_ITEM,				0,FRM__MAKEUPBAG,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_00_DAT,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,216*16,30*16,216,
	SC_MOVE_CAMERA_TO,			197*16,18*16,200,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_CARRY_ITEM,				0,FMA_ACTOR_BB,

// Scene 7 - BB gives new task
	SC_KILL_FX,					1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SITLOOKLEFT,1,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_01_DAT,


// Scene 8 - SB Leaves
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_02_DAT,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_RUBHEAD,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_STOP
};

/*****************************************************************************/
/*** C3 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC3EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_02_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_SET_ITEM,				0,FRM__UNIFORM,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,14*16,30*16,104,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_00_DAT,

// Scene 3 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_SIT,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,208*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SIT,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,211*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			197*16,18*16,
	SC_WAIT_ON_TIMER,			5,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SITLOOKLEFT,1,
	SC_WAIT_ON_TIMER,			5,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_01_DAT,
	SC_WAIT_ON_TIMER,			30,

// Scene 4 - SB enter by door, and runs over to them
	SC_MOVE_CAMERA_TO,			235*16,18*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_HIDE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,220*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_HIDE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,204*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,208*16,30*16,248,
	SC_MOVE_CAMERA_TO,			197*16,18*16,200,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,

// BB removes shade
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_UNHIDE,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_BB,
	SC_WAIT_ON_TIMER,			30,
	SC_CARRY_ITEM,				0,FMA_ACTOR_BB,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_02_DAT,

// Scene 8 - SB Leaves
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_RUBHEAD,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

	SC_STOP
};

/*****************************************************************************/
/*** C4 End FMA **************************************************************/
/*****************************************************************************/

static const int s_FMAC4EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_02_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			369*16,18*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,405*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_SET_ITEM,				0,FRM__KELP_BAR,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,370*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,382*16,30*16,92,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,

	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_00_DAT,
// SB throws bar thru window
	SC_THROW_ITEM_TO_ACTOR,		0,FMA_ACTOR_BB,64,120,
	SC_WAIT_ON_THROWN_ITEM,		0,

// BB comes to Porthole
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_BB,374*16,30*16,60,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_BB,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_01_DAT,

// Scene 8 - SB Leaves
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_02_DAT,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,420*16,30*16,40,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_RUBHEAD,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

	SC_STOP
};

/*****************************************************************************/
/*** C5 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC5EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_03_DAT,
	SC_USE_LEVEL,				25,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_MAP,
	SC_START,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_SET_ITEM,				0,FRM__TOOLBOX,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,16*16,30*16,96,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_00_DAT,

// Scene 3 - SB Fixing TV
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,224*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,220*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,
	SC_SET_ITEM,				0,FRM__AUTOGRAPH,
	SC_CARRY_ITEM,				0,FMA_ACTOR_BB,

	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,206*16,30*16,

	SC_SNAP_CAMERA_TO,			197*16,18*16,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_FIXTV,1,
	SC_WAIT_ON_TIMER,			60,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_01_DAT,
	SC_WAIT_ON_TIMER,			60,

// Scene 4 - Goes to BB, and collects his prize
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,216*16,30*16,44,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_02_DAT,
	SC_CARRY_ITEM,				0,FMA_ACTOR_SPONGEBOB,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_QUICKEXIT,0,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_ACTOR_ANIM,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 5 - SB leaves Shady Shoals
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_TIMER,			30,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,4*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,40*16,30*16,72,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 6 - Back in shady
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_SIT,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,208*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_SIT,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,211*16,(30*16)+8,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SNAP_CAMERA_TO,			197*16,18*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 7 - TV goes pop
	SC_CREATE_FX,				0,207*16,28*16,CFX::FX_TYPE_TV_EXPLODE,
	SC_WAIT_ON_TIMER,			60*3,

// Scene 8 - Outside, MM & BB SCREEEEEEEEEEEEEEEAM
	SC_SNAP_CAMERA_TO,			4*16,18*16,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_03_DAT,

	SC_STOP
};

/*****************************************************************************/
/*** Plankton FMA ************************************************************/
/*****************************************************************************/

static const int s_FMAPlanktonScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_PLANKTON_DAT,
	SC_USE_LEVEL,				26,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_GAME,
	SC_START,


	SC_SNAP_CAMERA_TO,			0*16,5*16,

	SC_WAIT_ON_TIMER,			60*2,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,0*16,18*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,18*16,18*16,72,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,32*16,18*16,56,
	SC_MOVE_CAMERA_TO,			21*16,5*16,84,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_PLANKTON,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_PLANKTON,42*16,18*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_PLANKTON,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_PLANKTON,true,

	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_PLANKTON_DAT,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,80*16,18*16,192,
	SC_MOVE_CAMERA_TO,			44*16,5*16,92,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

	SC_SNAP_CAMERA_TO,			94*16,5*16,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,114*16,18*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,108*16,18*16,24,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,

	SC_WAIT_ON_TIMER,			60,

	SC_STOP
};

/*****************************************************************************/
/*** Part FMA ****************************************************************/
/*****************************************************************************/

static const int s_FMAPartyScript[]=
{
	// Init
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_PARTY_DAT,
	SC_USE_PARTY,
	SC_SET_NEXT_SCENE,			FMA_NEXT_SCENE_CREDITS,
	SC_SNAP_CAMERA_TO,			0,0,
	SC_START,

	// Party scene
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,262,203,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,188,216,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,155,218,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_GARY,88,233,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_GARY,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_GARY,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_PLANKTON,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_PLANKTON,415,230,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_PLANKTON,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_PLANKTON,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_PATRICK,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_PATRICK,312,201,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_PATRICK,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_PATRICK,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_KRABS,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_KRABS,460,224,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_KRABS,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_KRABS,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SQUIDWARD,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SQUIDWARD,390,220,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SQUIDWARD,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SQUIDWARD,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SANDY,FMA_ANIM_IDLE,1,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SANDY,250,180,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SANDY,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SANDY,true,

	SC_WAIT_ON_TIMER,			60*2,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_PARTY_DAT,
	SC_SET_GLOBAL_ANIMATION,	false,
	SC_WAIT_ON_TIMER,			60*10,
	SC_STOP
};


static const int	*s_fmaScripts[CFmaScene::NUM_FMA_SCRIPTS]=
{
	s_FMAIntroScript,
	s_FMAC1EndScript,
	s_FMAC2EndScript,
	s_FMAC3EndScript,
	s_FMAC4EndScript,
	s_FMAC5EndScript,
	s_FMAPlanktonScript,
	s_FMAPartyScript,
};

static CFmaScene::FMA_SCRIPT_NUMBER	s_chosenScript=CFmaScene::FMA_SCRIPT__CH4FINISHED;//FMA_SCRIPT__INTRO;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::init()
{
	int			i;
	ACTOR_DATA	*actor;

	m_level=NULL;

	CThingManager::init();
	CConversation::init();

	m_cameraPos.vx=30;
	m_cameraPos.vy=280;
	m_cameraMoving=false;
	m_readyToShutdown=false;

	CActorPool::Reset();
	actor=m_actorData;
	for(i=0;i<FMA_NUM_ACTORS;i++)
	{
		for (int b=0; b<2; b++)
		{ // Load banks
			actor->m_gfx[b]=0;
			if (s_actorGraphicsData[i].m_file[b]) 
			{
				actor->m_gfx[b]=CActorPool::GetActor(s_actorGraphicsData[i].m_file[b]);
			}
		}

		actor->m_active=false;
		actor->m_animState=FMA_ANIM_IDLE;
		actor->m_animLoop=0;
		actor->m_animFrame=0;
		actor->m_pos.vx=0;
		actor->m_pos.vy=0;
		actor->m_moving=false;
		actor->m_facing=0;
		actor++;
	}

	for (i=0; i<FMA_FX_COUNT; i++)
	{
		m_FXTable[i]=0;
	}
	for (i=0; i<FMA_ITEM_MAX; i++)
	{
		m_itemData[i].m_visible=false;
		m_itemData[i].m_Actor=-1;
		m_itemData[i].m_TargetActor=-1;
	}

	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();

	m_frameCount=0;

	m_scriptRunning=true;
	m_pc=s_fmaScripts[s_chosenScript];

	m_musicPlaying=false;
	m_tuneLoaded=false;

	m_globalAnimationFlag=true;

	m_stillProcessingCommand=false;
	m_doOtherProcessing=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::shutdown()
{
	for(int i=0;i<FMA_NUM_ACTORS;i++)
	{
		for (int b=0; b<2; b++)
		{
			if (m_actorData[i].m_gfx[b]) delete m_actorData[i].m_gfx[b];
		}
	}
	CActorPool::Reset();

	if(m_level)
	{
		m_level->shutdown();
		delete m_level;
	}
	if(m_party)
	{
		m_party->shutdown();
		delete m_party;
	}

	CSoundMediator::dumpSong();

	CConversation::shutdown();
	CThingManager::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::render()
{
	int			i;
	ACTOR_DATA	*actor;

	CGameScene::setCameraMtx();

	CThingManager::renderAllThings();
	CConversation::render();

	if(m_level)
	{
		m_level->render();
	}
	else if(m_party)
	{
		m_party->render();
	}

	actor=m_actorData;
	for(i=0;i<FMA_NUM_ACTORS;i++)
	{
		if(actor->m_active)
		{
			// need clipping here
			DVECTOR	pos;
			pos.vx=actor->m_pos.vx-m_cameraPos.vx;
			pos.vy=actor->m_pos.vy-m_cameraPos.vy;
			int	AnimBank=s_actorGraphicsData[i].m_anims[actor->m_animState].Bank;
			int	AnimNo=s_actorGraphicsData[i].m_anims[actor->m_animState].Anim;
			actor->m_gfx[AnimBank]->Render(pos,AnimNo,actor->m_animFrame,actor->m_facing);
		}
		actor++;
	}

	for (i=0; i<FMA_ITEM_MAX; i++)
	{
		sItemData	*item=&m_itemData[i];
		if(item->m_visible)
		{
			DVECTOR	pos;
			pos.vx=item->m_Pos.vx-m_cameraPos.vx;
			pos.vy=item->m_Pos.vy-m_cameraPos.vy;
			CGameScene::getSpriteBank()->printFT4(item->m_Frame,pos.vx,pos.vy,item->m_facing,0,OTPOS__PICKUP_POS-3);
		}
	}

	CActorPool::CleanUpCache();

#if defined (__USER_paul__) || defined (__USER_daveo__)
FontBank	*font;
char		buf[20];
font=new ("Font") FontBank();
font->initialise(&standardFont);
sprintf(buf,"%d,%d",m_cameraPos.vx,m_cameraPos.vy);
font->print(400,30,buf);
font->dump();
delete font;
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::think(int _frames)
{
#if defined (__USER_paul__) || defined (__USER_daveo__)
	if (PadGetHeld(0)&PAD_L1)
	{
		if(PadGetHeld(0)&PAD_UP)	m_cameraPos.vy-=16;
		if(PadGetHeld(0)&PAD_DOWN)	m_cameraPos.vy+=16;
		if(PadGetHeld(0)&PAD_LEFT)	m_cameraPos.vx-=16;
		if(PadGetHeld(0)&PAD_RIGHT)	m_cameraPos.vx+=16;
	}
#endif

	if(!CFader::isFading()&&m_tuneLoaded&&!m_musicPlaying)
	{
		CSoundMediator::playSong();
		m_musicPlaying=true;
	}

	if(PadGetHeld(0) & PAD_START)
	{ // Give player an exit option!!
			m_scriptRunning=false;
			m_doOtherProcessing=true;
	}

	if(m_scriptRunning==false&&!m_readyToShutdown)
	{
		startShutdown();
	}

	for(int i=0;i<_frames;i++)
	{
		if(m_scriptRunning)
		{
			int			i;
			ACTOR_DATA	*actor;

			actor=m_actorData;
			for(i=0;i<FMA_NUM_ACTORS;i++)
			{
				if(actor->m_active)
				{
					// Move actor?
					if(actor->m_moving)
					{
						int	totalFrames,currentFrame;
						totalFrames=actor->m_endMoveFrame-actor->m_startMoveFrame;
						currentFrame=totalFrames-(actor->m_endMoveFrame-m_frameCount);
						if(currentFrame==0)
						{
							actor->m_pos=actor->m_startMovePos;
						}
						else if(currentFrame==totalFrames)
						{
							actor->m_pos=actor->m_endMovePos;
							actor->m_moving=false;
						}
						else
						{
							actor->m_pos.vx=actor->m_startMovePos.vx+(((actor->m_endMovePos.vx-actor->m_startMovePos.vx)*currentFrame)/totalFrames);
							actor->m_pos.vy=actor->m_startMovePos.vy+(((actor->m_endMovePos.vy-actor->m_startMovePos.vy)*currentFrame)/totalFrames);
						}
					}
				}

				// Anim
				if(m_globalAnimationFlag)
				{
					actor->m_animFrame++;
					int	AnimBank=s_actorGraphicsData[i].m_anims[actor->m_animState].Bank;
					int	AnimNo=s_actorGraphicsData[i].m_anims[actor->m_animState].Anim;
					int	LastFrame=actor->m_gfx[AnimBank]->getFrameCount(AnimNo)-1;
					if(actor->m_animFrame>=LastFrame)
					{
						if (actor->m_animLoop)
						{ // Loop anim
							actor->m_animFrame=0;
						}
						else
						{ // hold on last frame
							actor->m_animFrame=LastFrame;
						}
						
					}
				}

				actor++;
			}

			// Move Camera
			if(m_cameraMoving)
			{
				int	totalFrames,currentFrame;
				totalFrames=m_endCameraFrame-m_startCameraFrame;
				currentFrame=totalFrames-(m_endCameraFrame-m_frameCount);
				if(currentFrame==0)
				{
					m_cameraPos=m_startCameraPos;
				}
				else if(currentFrame==totalFrames)
				{
					m_cameraPos=m_endCameraPos;
					m_cameraMoving=false;
				}
				else
				{
					m_cameraPos.vx=m_startCameraPos.vx+(((m_endCameraPos.vx-m_startCameraPos.vx)*currentFrame)/totalFrames);
					m_cameraPos.vy=m_startCameraPos.vy+(((m_endCameraPos.vy-m_startCameraPos.vy)*currentFrame)/totalFrames);
				}
			}
			// Handle Item
			for (i=0; i<FMA_ITEM_MAX; i++)
			{
				sItemData	*item=&m_itemData[i];
				if(item->m_TargetActor!=-1)
				{
					// Being thrown to an actor
					int	totalFrames,currentFrame;
					totalFrames=item->m_endMoveFrame-item->m_startMoveFrame;
					currentFrame=totalFrames-(item->m_endMoveFrame-m_frameCount);
					if(currentFrame==0)
					{
						item->m_Pos=item->m_startPos;
					}
					else if(currentFrame>=totalFrames)
					{
						item->m_Pos=item->m_TargetPos;
						item->m_Actor=item->m_TargetActor;
						item->m_TargetActor=-1;
					}
					else
					{
						int	sin,yoff;
						item->m_Pos.vx=item->m_startPos.vx+(((item->m_TargetPos.vx-item->m_startPos.vx)*currentFrame)/totalFrames);
						item->m_Pos.vy=item->m_startPos.vy+(((item->m_TargetPos.vy-item->m_startPos.vy)*currentFrame)/totalFrames);
						sin=((currentFrame*2048)/totalFrames);
						yoff=-(msin(sin)*item->m_throwArcHeight)>>12;
						item->m_Pos.vy+=yoff;
					}
				}
				else if (item->m_Actor!=-1)
				{
					ACTOR_DATA					*actor=&m_actorData[item->m_Actor];
					const ACTOR_GRAPHICS_DATA	*actorGraphics=&s_actorGraphicsData[item->m_Actor];
						
					// Being carried by an actor
					item->m_Pos=actor->m_pos;
					item->m_facing=actor->m_facing;
					if (item->m_facing)
					{
						item->m_Pos.vx-=actorGraphics->m_ItemOfs.vx;
					}
					else
					{
						item->m_Pos.vx+=actorGraphics->m_ItemOfs.vx;
					}
					item->m_Pos.vy+=actorGraphics->m_ItemOfs.vy;
				}
			}

			// Process script
			do
			{
				if(!m_stillProcessingCommand)
				{
					startNextScriptCommand();
				}
				if(m_stillProcessingCommand)
				{
					processCurrentScriptCommand();
				}
			}
			while(!m_doOtherProcessing);
		}

		m_frameCount++;
	}

	CThingManager::thinkAllThings(_frames);
	CConversation::think(_frames);
	
	if(m_level)
	{
		m_level->setCameraCentre(m_cameraPos);
		m_level->think(_frames);
	}
	else if(m_party)
	{
		m_party->think(_frames);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CFmaScene::readyToShutdown()
{
	return m_readyToShutdown&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::selectFma(FMA_SCRIPT_NUMBER _fma)
{
	s_chosenScript=_fma;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startShutdown()
{
	CFader::setFadingOut();
	m_readyToShutdown=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startNextScriptCommand()
{
	m_stillProcessingCommand=false;
	m_doOtherProcessing=false;

	switch(*m_pc)
	{
		case SC_USE_LEVEL:
			ASSERT(!m_level);
			ASSERT(!m_party);
			m_pc++;
			m_level=new ("FMALevel") CLevel();
			m_level->init(*(m_pc++));
			m_tuneLoaded=true;
			break;

		case SC_USE_PARTY:				// 
			ASSERT(!m_level);
			ASSERT(!m_party);
			m_pc++;
			m_party=new ("FMAParty") CPartyScene();
			m_party->init();
			break;

		case SC_SET_NEXT_SCENE:			// nextScene
			m_pc++;
			GameState::setNextScene(s_nextGameSceneTable[*(m_pc++)]);
			break;

		case SC_SNAP_CAMERA_TO:			// x,y
			m_pc++;
			m_cameraPos.vx=*m_pc++;
			m_cameraPos.vy=*m_pc++;
			break;

		case SC_MOVE_CAMERA_TO:			// x,y,frames
			m_pc++;
			m_cameraMoving=true;
			m_startCameraPos=m_cameraPos;
			m_startCameraFrame=m_frameCount;
			m_endCameraPos.vx=*m_pc++;
			m_endCameraPos.vy=*m_pc++;
			m_endCameraFrame=m_frameCount+*m_pc++;
			break;
	
		case SC_REGISTER_CONVERSATION:	// scriptId
			m_pc++;
			CConversation::registerConversationScript((FileEquate)*(m_pc++));
			break;

		case SC_WAIT_ON_TIMER:			// frames
			m_timer=*(m_pc+1);
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_ACTOR_STOP:		// actor
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_ACTOR_ANIM:		// actor
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CONVERSATION:	// scriptId
			CConversation::trigger((FileEquate)*(m_pc+1));
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_THROWN_ITEM:	// item
			m_stillProcessingCommand=true;
			break;

		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_active=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_POSITION:		// actor,x,y
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_pos.vx=*m_pc++;
				actor->m_pos.vy=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_FACING:		// actor,facing
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_facing=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_animState=*m_pc++;
				actor->m_animLoop=*m_pc++;
				actor->m_animFrame=0;
			}
			break;

		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_startMoveFrame=m_frameCount;
				actor->m_startMovePos=actor->m_pos;
				actor->m_endMovePos.vx=*m_pc++;
				actor->m_endMovePos.vy=*m_pc++;
				actor->m_endMoveFrame=m_frameCount+*m_pc++;
				actor->m_moving=true;
			}
			break;

		case SC_SET_GLOBAL_ANIMATION:	// on/off
			{
				int	flag;
				m_pc++;
				flag=*(m_pc++);
				m_globalAnimationFlag=flag;
				if(m_party)m_party->setAnimation(flag);
			}
			break;

		case SC_CREATE_FX:
			{
				int	FXNo;
				int	FXType;
				DVECTOR	Pos;
				m_pc++;
				
				FXNo=*m_pc++;
				Pos.vx=*m_pc++;
				Pos.vy=*m_pc++;
				FXType=*m_pc++;

				m_FXTable[FXNo]=CFX::Create((CFX::FX_TYPE)FXType,Pos);
			}
			break;

		case SC_KILL_FX:
			{
				int	FXNo;
				m_pc++;
				
				FXNo=*m_pc++;
				m_FXTable[FXNo]->killFX();
			}
			break;

		case SC_SET_ITEM:				// item, Frame
			{
				sItemData	*item;
				m_pc++;
				item=&m_itemData[*m_pc++];
				item->m_Frame=*m_pc++;
			}
			break;
	

		case SC_CARRY_ITEM:				// item, actor
			{
				sItemData	*item;
				m_pc++;
				item=&m_itemData[*m_pc++];
				item->m_Actor=*m_pc++;
				item->m_visible=true;
			}
			break;

		case SC_HIDE_ITEM:				// item
			m_pc++;
			m_itemData[*m_pc++].m_visible=false;
			break;

		case SC_THROW_ITEM_TO_ACTOR:	// item,targetActor,arcHeight,frames
			{
				sItemData	*item;
				m_pc++;
				item=&m_itemData[*m_pc++];
				ASSERT(item->m_Actor!=-1);
				item->m_TargetActor=*m_pc++;
				ASSERT(item->m_Actor!=item->m_TargetActor);
				item->m_throwArcHeight=*(m_pc++);
				item->m_startMoveFrame=m_frameCount;
				item->m_endMoveFrame=m_frameCount+*m_pc++;

				// Calc the positions
				const ACTOR_GRAPHICS_DATA	*actorGraphics=&s_actorGraphicsData[item->m_TargetActor];
				item->m_TargetPos=m_actorData[item->m_TargetActor].m_pos;
				if(m_actorData[item->m_TargetActor].m_facing)
				{
					item->m_TargetPos.vx-=actorGraphics->m_ItemOfs.vx;
				}
				else
				{
					item->m_TargetPos.vx+=actorGraphics->m_ItemOfs.vx;
				}
				item->m_TargetPos.vy+=actorGraphics->m_ItemOfs.vy;
				item->m_startPos=item->m_Pos;
			}
			break;

		case SC_START:					// 
			m_pc++;
			CFader::setFadingIn();
			m_doOtherProcessing=true;
			break;

		case SC_STOP:					// 
			m_scriptRunning=false;
			m_doOtherProcessing=true;
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::processCurrentScriptCommand()
{
	switch(*m_pc)
	{
		case SC_USE_LEVEL:				// levelNumber
		case SC_USE_PARTY:				// 
		case SC_SET_NEXT_SCENE:			// nextScene
		case SC_SNAP_CAMERA_TO:			// x,y
		case SC_MOVE_CAMERA_TO:			// x,y,frames
		case SC_REGISTER_CONVERSATION:	// scriptId
		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
		case SC_SET_ACTOR_POSITION:		// actor,x,y
		case SC_SET_ACTOR_FACING:		// actor,facing
		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
		case SC_SET_GLOBAL_ANIMATION:	// on/off
		case SC_START:					// 
		case SC_STOP:					// 
			ASSERT(!"Shouldn't be here..");
			break;

		case SC_WAIT_ON_TIMER:			// frames
			if(m_timer--==0)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;
		
		case SC_WAIT_ON_ACTOR_STOP:		// actor
			if(!m_actorData[*(m_pc+1)].m_moving)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		case SC_WAIT_ON_ACTOR_ANIM:		// actor
			{
				int							ThisActor=*(m_pc+1);
				ACTOR_DATA					*actor=&m_actorData[ThisActor];
				const ACTOR_GRAPHICS_DATA	*actorGraphics=&s_actorGraphicsData[ThisActor];
				int	AnimBank=actorGraphics->m_anims[actor->m_animState].Bank;
				int	AnimNo=actorGraphics->m_anims[actor->m_animState].Anim;
				int	LastFrame=actor->m_gfx[AnimBank]->getFrameCount(AnimNo)-1;
				if(actor->m_animFrame>=LastFrame)
				{
					m_pc+=2;
					m_stillProcessingCommand=false;
				}
				else
				{
					m_doOtherProcessing=true;
				}
			}
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			if(!m_cameraMoving)
			{
				m_pc++;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;


		case SC_WAIT_ON_CONVERSATION:	// scriptId
			if(!CConversation::isActive())
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		case SC_WAIT_ON_THROWN_ITEM:	// item
			if(m_itemData[*(m_pc+1)].m_TargetActor==-1)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*===========================================================================
 end */