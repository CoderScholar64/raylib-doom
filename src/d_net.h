// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Networking stuff.
//
//-----------------------------------------------------------------------------


#ifndef __D_NET__
#define __D_NET__

#include "d_player.h"

#include "doomtype.h"

//
// Network play related stuff.
// There is a data struct that stores network
//  communication related stuff, and another
//  one that defines the actual packets to
//  be transmitted.
//

#define DOOMCOM_ID		0x12345678l

// Max computers/players in a game.
#define MAXNETNODES		8


// Networking and tick handling related.
#define BACKUPTICS		12

typedef enum
{
    CMD_SEND	= 1,
    CMD_GET	= 2

} command_t;


//
// Network packet data.
//
typedef struct
{
    // High bit is retransmit request.
    d_uint  		checksum;
    // Only valid if NCMD_RETRANSMIT.
    byte		retransmitfrom;
    
    byte		starttic;
    byte		player;
    byte		numtics;
    ticcmd_t		cmds[BACKUPTICS];

} doomdata_t;




typedef struct
{
    // Supposed to be DOOMCOM_ID?
    d_long		id;
    
    // DOOM executes an int to execute commands.
    d_short		intnum;
    // Communication between DOOM and the driver.
    // Is CMD_SEND or CMD_GET.
    d_short		command;
    // Is dest for send, set by get (-1 = no packet).
    d_short		remotenode;
    
    // Number of bytes in doomdata to be sent
    d_short		datalength;

    // Info common to all nodes.
    // Console is allways node 0.
    d_short		numnodes;
    // Flag: 1 = no duplication, 2-5 = dup for slow nets.
    d_short		ticdup;
    // Flag: 1 = send a backup tic in every packet.
    d_short		extratics;
    // Flag: 1 = deathmatch.
    d_short		deathmatch;
    // Flag: -1 = new game, 0-5 = load savegame
    d_short		savegame;
    d_short		episode;	// 1-3
    d_short		map;		// 1-9
    d_short		skill;		// 1-5

    // Info specific to this node.
    d_short		consoleplayer;
    d_short		numplayers;
    
    // These are related to the 3-display mode,
    //  in which two drones looking left and right
    //  were used to render two additional views
    //  on two additional computers.
    // Probably not operational anymore.
    // 1 = left, 0 = center, -1 = right
    d_short		angleoffset;
    // 1 = drone
    d_short		drone;

    // The packet data to be sent.
    doomdata_t		data;
    
} doomcom_t;



// Create any new ticcmds and broadcast to other players.
void NetUpdate (void);

// Broadcasts special packets to other players
//  to notify of game exit
void D_QuitNetGame (void);

//? how many ticks to run?
void TryRunTics (void);


#endif

//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

