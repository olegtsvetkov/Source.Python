/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

#ifndef _CONSTANTS_CONST_WRAP_PYTHON_ORANGEBOX_H
#define _CONSTANTS_CONST_WRAP_PYTHON_ORANGEBOX_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "const.h"


//-----------------------------------------------------------------------------
// Expose SolidFlags_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_solid_flags(T SolidFlags)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose RenderFx_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_render_effects(T RenderEffects)
{
	RenderEffects.value("DISTORT", kRenderFxDistort);
	RenderEffects.value("HOLOGRAM", kRenderFxHologram);
	RenderEffects.value("EXPLODE", kRenderFxExplode);
	RenderEffects.value("CLAMP_MIN_SCALE", kRenderFxClampMinScale);
	RenderEffects.value("ENV_RAIN", kRenderFxEnvRain);
	RenderEffects.value("ENV_SNOW", kRenderFxEnvSnow);
	RenderEffects.value("SPOTLIGHT", kRenderFxSpotlight);
	RenderEffects.value("RAGDOLL", kRenderFxRagdoll);
	RenderEffects.value("PULSE_FAST_WIDER", kRenderFxPulseFastWider);
}


//-----------------------------------------------------------------------------
// Expose Collision_Group_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_collision_groups(T CollisionGroups)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose EntityEffects.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_entity_effects(T EntityEffects)
{
	// Nothing specific to OrangeBox...
}


#endif // _CONSTANTS_CONST_WRAP_PYTHON_ORANGEBOX_H
