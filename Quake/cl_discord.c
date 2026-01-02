/*
Copyright (C) 2024 APAMk2

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// cl_discord.c  -- client discord status management


#include "quakedef.h"
#include "steam.h"
#include "cl_discord.h"
#include "discord_rpc.h"

#include <time.h>

static const char* APPLICATION_ID = "1291424691222876262";
cvar_t	cl_discordrpc = { "cl_discordrpc", "1", CVAR_ARCHIVE };

static void DRPC_HandleReady (const DiscordUser* connUser)
{
	Con_Printf ("Discord: connected to user %s - %s\n", connUser->username, connUser->userId);
}

static void DRPC_HandleDisconnect (int errcode, const char* message)
{
	Con_Printf ("Discord: disconnected (reason - %d: %s)\n", errcode, message);
}

static void DRPC_HandleError (int errcode, const char* message)
{
	Con_Warning ("Discord: error (reason - %d: %s)\n", errcode, message);
}

void DRPC_Init (void)
{
	char steamID[128];
	itoa (QUAKE_STEAM_APPID, steamID, 10);

	DiscordEventHandlers handlers;
	memset (&handlers, 0, sizeof (handlers));
	handlers.ready = DRPC_HandleReady;
	handlers.disconnected = DRPC_HandleDisconnect;
	handlers.errored = DRPC_HandleError;
	Discord_Initialize (APPLICATION_ID, &handlers, 1, steamID);

	Cvar_RegisterVariable (&cl_discordrpc);

	DRPC_ClearStatus ();
}

void DRPC_ClearStatus (void)
{
	Discord_ClearPresence ();
}

void DRPC_SetStatus_Menu (void)
{
	if (cl_discordrpc.value)
	{
		char state[128] = "In main menu";
		
		DiscordRichPresence discordPresence;
		memset (&discordPresence, 0, sizeof (discordPresence));
		discordPresence.state = state;
		discordPresence.details = "";
		discordPresence.largeImageKey = "ironwail";
		discordPresence.instance = 0;
		Discord_UpdatePresence (&discordPresence);
	}
	else
	{
		DRPC_ClearStatus ();
	}
}

void DRPC_SetStatus_SinglePlayer (int skill, int hp, int ap, int startTime, const char* map, const char* mapName)
{
	if (cl_discordrpc.value)
	{
		char state[128];
		char details[128];

		q_snprintf (state, sizeof (state), "%s: %s", map, mapName);
		q_snprintf (details, sizeof (details), "SP|SK:%d|HP:%d, AP:%d", skill, hp, ap);

		DiscordRichPresence discordPresence;
		memset (&discordPresence, 0, sizeof (discordPresence));
		discordPresence.state = state;
		discordPresence.details = details;
		discordPresence.startTimestamp = (int)time(0) - startTime;
		discordPresence.largeImageKey = "ironwail";
		discordPresence.instance = 0;
		Discord_UpdatePresence (&discordPresence);
	}
	else
	{
		DRPC_ClearStatus ();
	}
}

void DRPC_SetStatus_Multiplayer (int skill, int hp, int ap, int startTime, const char* map, const char* mapName, int gametype, int players, int maxplayers)
{
	if (cl_discordrpc.value)
	{
		char state[128];
		char details[128];

		q_snprintf (state, sizeof (state), "%s: %s", map, mapName);
		q_snprintf (details, sizeof (details), "%s|SK:%d|HP:%d, AP:%d", gametype == GAME_DEATHMATCH ? "DM" : "COOP", skill, hp, ap);

		DiscordRichPresence discordPresence;
		memset (&discordPresence, 0, sizeof (discordPresence));
		discordPresence.state = state;
		discordPresence.details = details;
		discordPresence.startTimestamp = (int)time (0) - startTime;
		discordPresence.largeImageKey = "ironwail";
		discordPresence.partySize = players;
		discordPresence.partyMax = maxplayers;
		discordPresence.instance = 0;
		Discord_UpdatePresence (&discordPresence);
	}
	else
	{
		DRPC_ClearStatus ();
	}
}

void DRPC_Shutdown (void)
{
	Discord_Shutdown ();
}