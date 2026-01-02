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

#ifndef _DISCORD_H_
#define _DISCORD_H_

extern cvar_t cl_discordrpc;

void				DRPC_Init (void);
void				DRPC_ClearStatus (void);
void				DRPC_SetStatus_Menu (void);
void				DRPC_SetStatus_SinglePlayer (int skill, int hp, int ap, int startTime, const char* map, const char* mapName);
void				DRPC_SetStatus_Multiplayer (int skill, int hp, int ap, int startTime, const char* map, const char* mapName, int gametype, int players, int maxplayers);
void				DRPC_Shutdown (void);

#endif /*_DISCORD_H_*/