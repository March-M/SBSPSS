/*=========================================================================

	player.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PLAYER_H__
#define __PLAYER_PLAYER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#include "gfx/actor.h"


#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif

#ifndef	__PLAYER_PSTATES_H__
#include "player\pstates.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	PLAYER_MODE_BASICUNARMED,
	PLAYER_MODE_FULLUNARMED,
	PLAYER_MODE_BALLOON,
	PLAYER_MODE_BUBBLE_MIXTURE,
	PLAYER_MODE_NET,
	PLAYER_MODE_CORALBLOWER,
	PLAYER_MODE_JELLY_LAUNCHER,

	PLAYER_MODE_DEAD,

	PLAYER_MODE_FLY,

	PLAYER_MODE_CART,
	PLAYER_MODE_SWALLOW,

	NUM_PLAYERMODES,
}PLAYER_MODE;

typedef enum
{
	STATE_IDLE,
	STATE_IDLETEETER,
	STATE_JUMP,
	STATE_SPRINGUP,
	STATE_RUN,
	STATE_FALL,
	STATE_FALLFAR,
	STATE_HITGROUND,
	STATE_BUTTBOUNCE,
	STATE_BUTTFALL,
	STATE_BUTTLAND,
	STATE_BUTTBOUNCEUP,
	STATE_SOAKUP,
	STATE_GETUP,
	STATE_LOOKDOWN,
	STATE_LOOKDOWNRELAX,
	STATE_LOOKUP,
	STATE_LOOKUPRELAX,
	STATE_JUMPBACK,
	STATE_CART,
	STATE_FLOAT,
	STATE_CELEBRATE,
	STATE_SWALLOW,

	NUM_STATES,
}PLAYER_STATE;

typedef enum
{
	PLAYER_ADDON_NET,
	PLAYER_ADDON_CORALBLOWER,
	PLAYER_ADDON_JELLYLAUNCHER,
	PLAYER_ADDON_BUBBLEWAND,
	PLAYER_ADDON_JELLYFISHINNET,
	PLAYER_ADDON_GLOVE,

	NUM_PLAYER_ADDONS,
	NO_ADDON,
}PLAYER_ADDONS;

enum
{
	FACING_LEFT=+1,
	FACING_RIGHT=-1,
};

typedef enum
{
	DAMAGE__NONE,			// put in so that an item can push the user but not damage them
	DAMAGE__FALL,
	DAMAGE__ELECTROCUTION,
	DAMAGE__LAVA,
	DAMAGE__HIT_ENEMY,
	DAMAGE__SHOCK_ENEMY,
	DAMAGE__GAS_ENEMY,
	DAMAGE__POISON_ENEMY,
	DAMAGE__SWALLOW_ENEMY,
	DAMAGE__PINCH_ENEMY,
	DAMAGE__SQUASH_ENEMY,
	DAMAGE__BURN_ENEMY,
	DAMAGE__BITE_ENEMY,
	DAMAGE__COLLISION_DAMAGE,
	DAMAGE__KILL_OUTRIGHT,
}DAMAGE_TYPE;

typedef enum
{
	DEATHTYPE__NORMAL,
	DEATHTYPE__DRYUP,
	DEATHTYPE__SQUASH,
	DEATHTYPE__LIQUID,
} DEATH_TYPE;

typedef enum
{
	REACT__LEFT=-1,
	REACT__UP=0,
	REACT__RIGHT=+1,
	REACT__GET_DIRECTION_FROM_THING=123,
	REACT__NO_REACTION=234,
}REACT_DIRECTION;


// The input from the control pad is remapped to this rather than keeping it in the
// normal pad format. This allows us to store all input in one byte ( as opposed to
// two bytes ) for demo recording and means that the player state codes don't have
// to keep using CPadConfig to remap the controls internally.
typedef enum
{
	PI_NONE			=0,
	PI_UP			=1<<0,
	PI_DOWN			=1<<1,
	PI_LEFT			=1<<2,
	PI_RIGHT		=1<<3,
	PI_JUMP			=1<<4,
	PI_FIRE			=1<<5,
	PI_CATCH		=1<<6,
	PI_WEAPONCHANGE	=1<<7,
}PLAYERINPUT;


// For getAttackState()
typedef enum
{
	ATTACK_STATE__NONE,
	ATTACK_STATE__KARATE,
	ATTACK_STATE__BUTT_BOUNCE,
} ATTACK_STATE;




// Special anim that means SB will not get drawn
// Just pass it to setAnimNo() like a normal anim
#define ANIM_SPONGEBOB_DONOTDRAW		-1



/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayer : public CPlayerThing
{
public:
	enum
	{
		MAX_LIVES=99,
		
		WATERLEVELSHIFT=5,
		WATERMAXHEALTH=(255<<WATERLEVELSHIFT),
		WATERMINSOAKUPLEVEL=(245<<WATERLEVELSHIFT),					// SB won't auto soak unless water is lower than this

		WATER_COUNTER_STARTSCALE=50,								// Scale of text
		WATER_COUNTER_ENDSCALE=511,
		WATER_COUNTER_R1=59,										// Colour of text
		WATER_COUNTER_G1=118,
		WATER_COUNTER_B1=253,
		WATER_COUNTER_R2=255,
		WATER_COUNTER_G2=28,
		WATER_COUNTER_B2=17,
		WATER_COUNTER_SECONDTIME=15<<CPlayer::WATERLEVELSHIFT,		// Each digit lasts this many frames..
		WATER_COUNTER_MOVINGTIME=5<<CPlayer::WATERLEVELSHIFT,		// ..and spends this longing moving into position
	};

	typedef struct
	{
		s16			x1,y1;
		s16			x2,y2;
	} CameraBox;

	
	virtual void	init();
	virtual void	shutdown();
	virtual void	think(int _frames);
	void			detectHazardousSurface();
	virtual void	render();
	virtual int		dontKillDuringLevelRespawn()							{return true;}
	virtual void	shove(DVECTOR move);
	void			moveLeft();				// This is only for camera scroll right now
	void			moveRight();			//	"		"		"		"		"
	void			fall();					//	"		"		"		"		"
	void			buttFall();				//	"		"		"		"		"
	virtual int		moveVertical(int _moveDistance);
	virtual int		moveHorizontal(int _moveDistance);

	int				isTryingToConversateWithFriend()					{return m_allowConversation;}

	DVECTOR			getCameraPos()										{return m_cameraPos;}

	void			setCameraBox(CameraBox _cameraBox);
	void			setRespawnPos(DVECTOR _respawn)						{m_respawnPos=_respawn;}
	void			setRespawnPosAndRingTelephone(DVECTOR _respawn);

	// This isn't funny anymore.. :(
	int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
	int				getHeightFromPlatformNoGround(int _x,int _y,int _maxHeight=32);
	int				getHeightFromGroundNoPlatform(int _x,int _y,int _maxHeight=32);

	void			setLedgeLookAhead(int _lookAhead)					{m_ledgeLookAhead=_lookAhead;}

	void			addLife();
	void			addSpatula(int Count=1)								{m_numSpatulasHeld+=Count;}
	int				getSpatulasHeld()									{return m_numSpatulasHeld;}
	ATTACK_STATE	getAttackState();
	int				isRecoveringFromHit()								{return m_invincibleFrameCount!=0||m_currentMode==PLAYER_MODE_DEAD;}

	void			registerAddon(PLAYER_ADDONS _addon);
	DVECTOR			*getMoveVelocity()									{return &m_moveVelocity;}
	void			setMoveVelocity(const DVECTOR *_moveVelocity)		{m_moveVelocity=*_moveVelocity;}

public:
	void			setMode(PLAYER_MODE _mode);
	PLAYER_MODE		getMode()											{return m_currentMode;}
	int				getFacing();
	void			setFacing(int _facing);
	int				getAnimFrame();
	void			setAnimFrame(int _animFrame);
	int				getAnimFrameCount();
	int				getAnimNo();
	void			setAnimNo(int _animNo);

	void			springPlayerUp(int _springHeight=55);		// Springy branches
	void			floatPlayerUp();							// Geysers
	void			teleportTo(int _x,int _y);

private:
	void			playAnimFrameSfx(int _animNo,int _animFrame);
	void			calcCameraFocusPointTarget();

	DVECTOR			m_moveVelocity;
	int				m_allowConversation;



public:
	DVECTOR			getPlayerPos()										{return Pos;}
	void			setPlayerPos(DVECTOR *_pos)							{Pos=*_pos;}
	void			ignoreNewlyPressedButtonsOnPadThisThink();
	PLAYERINPUT		getPadInputHeld()									{return m_padInput;}
	PLAYERINPUT		getPadInputDown()									{return m_padInputDown;}
	int				tryingToManuallyPickupWeapon()						{return m_tryingToManuallyPickupWeapon;}
	int				tryingToAutomaticallyPickupWeapon()					{return m_tryingToAutomaticallyPickupWeapon;}
							
	void			inSoakUpState();
	void			takeDamage(DAMAGE_TYPE _damage,REACT_DIRECTION _reactDirection=REACT__UP,CThing *_thing=NULL);

	void			respawn();
	void			setCanExitLevelNow()								{m_canExitLevel=true;}
	int				getCanExitLevelNow()								{return m_canExitLevel;}
	void			collectedQuestItem();

	int				getLivesLeft();


public:
	typedef struct
	{
		int						m_frame;
		CSoundMediator::SFXID	m_sfxId;
	} AnimFrameSfx;
	void			renderSb(DVECTOR *_pos,int _animNo,int _animFrame);

	int				canDoLookAround();

	

private:
	typedef struct
	{
		int							m_animNumber;
		int							m_numAnimFrameSfx;
		const struct AnimFrameSfx	*m_animFrameSfx;
	} AnimSfx;
	static const AnimSfx	s_animSfx[];
	static const int		s_numAnimSfx;
	int				m_animFrame;
	int				m_animNo;
	CActorGfx		*m_actorGfx;

	int				m_lockCamera;
	DVECTOR			m_cameraPos;
	DVECTOR			m_currentCamFocusPointTarget;
	DVECTOR			m_currentCamFocusPoint;
	int				m_facing;
	int				m_padLookAroundTimer;
	int				m_ledgeLookAhead,m_lastLedgeLookAhead;
	int				m_ledgeLookOffset;
	int				m_ledgeLookTimer;


	enum
	{
		INVINCIBLE_FRAMES__START=200,	// Invincible for this many frames at start of life
		INVINCIBLE_FRAMES__HIT=120,		// Invincible for this many frames after taking damage
	};
	int				m_invincibleFrameCount;		// Initial invincibility and also invincibility after taking damage

	static class CPlayerMode	*s_playerModes[NUM_PLAYERMODES];
	class CPlayerMode			*m_currentPlayerModeClass;
	PLAYER_MODE					m_currentMode;
	PLAYER_MODE					m_lastModeBeforeDeath;

public:
	DEATH_TYPE		getDeathType()				{return m_deathType;}

private:
	int				m_numSpatulasHeld;
	int				m_healthWaterLevel;
	int				m_helmetSoundTimer;			// Timer for breathing sound when using the helmet


	void			dieYouPorousFreak(DEATH_TYPE _deathType=DEATHTYPE__NORMAL);
	DEATH_TYPE		m_deathType;


	void			updatePadInput();
protected:
	virtual PLAYERINPUT	readPadInput();
private:
	PLAYERINPUT			m_padInput;			// Controls that are being held down
	PLAYERINPUT			m_lastPadInput;		// Last frames controls
	PLAYERINPUT			m_padInputDown;		// Controls that were pressed this frame
	int					m_ignoreNewlyPressedButtonsOnPadThisThink;
	int					m_tryingToManuallyPickupWeapon;
	int					m_tryingToAutomaticallyPickupWeapon;

	
	// Various info about the current map
	CameraBox				m_cameraPosLimitBox;
	CameraBox				m_playerPosLimitBox;
	DVECTOR					m_respawnPos;
	int						m_canExitLevel;



	// Powerups
private:
	enum
	{
		SQUEAKY_BOOTS_TIME=60*10,
		INVINCIBILITY_RING_TIME=60*10,
		MAX_BUBBLE_BLOWER_AMMO=99,
		MAX_JELLY_LAUNCHER_AMMO=99,
		MAX_JELLFISH_IN_NET=5,
	};
public:
	void			giveSqueakyBoots()						{m_squeakyBootsTimer=SQUEAKY_BOOTS_TIME;}
	int				isWearingBoots()						{return m_squeakyBootsTimer;}
	void			giveInvincibilityRing()					{m_invincibilityRingTimer=INVINCIBILITY_RING_TIME;}
	void			giveDivingHelmet()						{m_divingHelmet=true;}
	int				isWearingDivingHelmet()					{return m_divingHelmet;}
															
	void			giveBubbleAmmo()						{m_bubbleAmmo+=10;if(m_bubbleAmmo>MAX_BUBBLE_BLOWER_AMMO)m_bubbleAmmo=MAX_BUBBLE_BLOWER_AMMO;}
	void			useOneBubble()							{m_bubbleAmmo--;}
	int				getBubbleAmmo()							{return m_bubbleAmmo;}
															
	void			giveJellyAmmo()							{m_jellyAmmo+=6;if(m_jellyAmmo>MAX_JELLY_LAUNCHER_AMMO)m_jellyAmmo=MAX_JELLY_LAUNCHER_AMMO;}
	void			useOneJelly()							{m_jellyAmmo--;}
	int				getJellyAmmo()							{return m_jellyAmmo;}
															
	void			giveJellyFishAmmo(u32 _colour);
	void			useOneJellyFishAmmo();
	int				isJellyFishAmmoFull();
	int				getJellyFishAmmo();
	u32				getColourOfNextJellyfishAmmo();

	void			setIsInWater(int _in)					{m_isInWater=_in;m_helmetSoundTimer=0;}
	int				getIsInWater()							{return m_isInWater;}
	int				getIsHelmetFullSoICanStopSoakingUp()	{return m_healthWaterLevel==WATERMAXHEALTH;}
	int				getIsHelmetSoFullThatIDontNeedToSoakUp(){return m_healthWaterLevel>=WATERMINSOAKUPLEVEL;}

	int				isHoldingNet()							{return m_currentMode==PLAYER_MODE_NET;}

	void			justButtBouncedABadGuy();	// Also fugly.. :/

	void			setFloating();

private:
	int				m_squeakyBootsTimer;
	int				m_invincibilityRingTimer;
	int				m_divingHelmet;
	int				m_bubbleAmmo;
	int				m_jellyAmmo;
	int				m_jellyfishAmmoCount;
	u32				m_jellyfishAmmoColours[MAX_JELLFISH_IN_NET];
	int				m_isInWater;

	// Platforms
public:
	void			setPlatform(CThing *_newPlatform);
	void			clearPlatform();
	CThing			*isOnPlatform()						{return m_platform;}

private:
	CThing			*m_platform;


	// Player collision size
public:
	enum
	{
		COLSIZE_BASE_WIDTH=30,
		COLSIZE_BASE_HEIGHT=60,
		HEIGHT_FOR_HEAD_COLLISION=64,
	};
	void			resetPlayerCollisionSizeToBase();
	void			setPlayerCollisionSize(int _x,int _y,int _w,int _h);
	void			getPlayerCollisionSize(int *_x,int *_y,int *_w,int *_h);



	// Graphical resources
public:
	class FontBank			*getFontBank()					{return m_fontBank;}
	class ScalableFontBank	*getScalableFontBank()			{return m_scalableFontBank;}

	enum
	{
		SB_UI_XBASE=40,
		SB_UI_YBASE=40,
		SB_UI_GAP_FROM_SPAT_COUNT_TO_PICKUPS=50,
		SB_UI_GAP_BETWEEN_ITEMS=5,

		POWERUPUI_ICONX=400,
		POWERUPUI_ICONY=40,
		POWERUPUI_TEXTX=440,
		POWERUPUI_TEXTY=37,
		POWERUPUI_OT=0,
	};

private:
	class FontBank			*m_fontBank;
	class ScalableFontBank	*m_scalableFontBank;

public:

	// Movement tracking for barrels

	s32				getMovement()						{return( m_xMove );}

private:
	s32				m_xMove;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PLAYER_H__ */

/*===========================================================================
 end */
