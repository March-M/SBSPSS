/*=========================================================================

	platdata.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef	__ANIM_CLAM_HEADER__
#include <ACTOR_CLAM_ANIM.h>
#endif

CNpcPlatform::NPC_PLATFORM_DATA CNpcPlatform::m_data[NPC_PLATFORM_TYPE_MAX] =
{
	{	// NPC_LINEAR_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		1,
		//512,
		2048,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_CIRCULAR_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_BUBBLE_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_FINITE_LIFE_RESPAWN,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_COLLAPSING_BUBBLE_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_FISH_HOOK_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_RETRACTING_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_GEYSER_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		8,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_BOBBING_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		0,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_FALLING_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		4,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_CART_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		4,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_FISH_HOOK_2_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_BRANCH_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_SEESAW_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_OILDRUM_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		2,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_CRATE_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_BOUNCE_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_DUAL_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		4,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_OILDRUM_GENERATOR
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		2,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		2,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_CRATE_GENERATOR
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_INFINITE_LIFE,
		2,
		NPC_PLATFORM_TIMER_NONE,
	},

	{	// NPC_PLAYER_BUBBLE_PLATFORM
		ACTORS_CLAM_SBK,
		ANIM_CLAM_SIDESNAP,
		3,
		128,
		true,
		DAMAGE__NONE,
		0,
		4,
		NPC_PLATFORM_FINITE_LIFE,
		0,
		NPC_PLATFORM_TIMER_NONE,
	},
};

CNpcPlatform::NPC_PLATFORM_UNIT_TYPE CNpcPlatform::mapEditConvertTable[NPC_PLATFORM_TYPE_MAX] =
{
	NPC_BUBBLE_PLATFORM,
	NPC_LINEAR_PLATFORM,
	NPC_FALLING_PLATFORM,
	NPC_CIRCULAR_PLATFORM,
	NPC_COLLAPSING_BUBBLE_PLATFORM,
	NPC_FISH_HOOK_PLATFORM,
	NPC_RETRACTING_PLATFORM,
	NPC_GEYSER_PLATFORM,
	NPC_BOBBING_PLATFORM,
	NPC_CART_PLATFORM,
	NPC_FISH_HOOK_2_PLATFORM,
	NPC_BRANCH_PLATFORM,
	NPC_SEESAW_PLATFORM,
	NPC_OILDRUM_PLATFORM,
	NPC_CRATE_PLATFORM,
	NPC_BOUNCE_PLATFORM,
	NPC_DUAL_PLATFORM,
	NPC_OILDRUM_GENERATOR,
	NPC_CRATE_GENERATOR,
	NPC_PLAYER_BUBBLE_PLATFORM,
};
