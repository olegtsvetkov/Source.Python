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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "eiface.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"


// Externals
extern IEngineTrace* enginetrace;


//---------------------------------------------------------------------------------
// IVEngineServer
//---------------------------------------------------------------------------------
// Visitor function
template<class T>
T IVEngineServer_Visitor(T cls)
{
	cls
		.def("get_time",
			&IVEngineServer::Time,
			"Returns a high precision timer for doing profiling work"
		)

		.def("multiplayer_end_game",
			&IVEngineServer::MultiplayerEndGame,
			"Matchmaking"
		)

		.def("create_fake_client_ex",
			&IVEngineServer::CreateFakeClientEx,
			reference_existing_object_policy()
		)
	;

	return cls;
}


//---------------------------------------------------------------------------------
// IEngineSound
//---------------------------------------------------------------------------------
inline void IEngineSound_EmitSound(IEngineSound* pEngineSound, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, 
		float flVolume, float flAttenuation, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL,
		tuple origins = tuple(), bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1)
{
	CUtlVector<Vector> *pUtlVecOrigins = NULL;
	CUtlVector<Vector> vecOrigins;
	if (len(origins) > 0)
	{
		pUtlVecOrigins = &vecOrigins;
		for(int i=0; i < len(origins); i++)
		{
			vecOrigins.AddToTail(extract<Vector>(origins[i]));
		}
	}
    
	pEngineSound->EmitSound(filter, iEntIndex, iChannel, pSample, flVolume, flAttenuation, iFlags, iPitch, 0, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}

// Visitor function
template<class T>
T IEngineSound_Visitor(T cls)
{
	return cls;
}


//---------------------------------------------------------------------------------
// IEngineTrace
//---------------------------------------------------------------------------------
inline int GetPointContents(const Vector &vecAbsPosition, IHandleEntity** ppEntity)
{
	return enginetrace->GetPointContents(vecAbsPosition, ppEntity);
}
