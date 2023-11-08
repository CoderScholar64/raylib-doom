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
// $Log:$
//
// DESCRIPTION:
//	System interface for sound.
//
//-----------------------------------------------------------------------------

#include "doomtype.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <math.h>

#include "z_zone.h"

#include "i_system.h"
#include "i_sound.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"

#include "doomdef.h"

#include "raylib.h"

#define SFX_CHANNELS          1
#define SFX_BYTES_PER_CHANNEL 1
#define SFX_BITS_PER_CHANNEL  SFX_BYTES_PER_CHANNEL * 8
#define SFX_SAMPLE_RATE       11025 // Hz

Sound sfx_audios[NUMSFX];
d_uint sfx_audio_amount = 0;
Sound* sfx_audio_table[NUMSFX];

void* getsfx( d_char* sfxname, Sound* sfxsound ) {
    unsigned char*      sfx;
    d_int  sfxlump;
    d_int  size;
    d_char name[20];

    snprintf(name, 20, "ds%s", sfxname);

    if ( W_CheckNumForName(name) == -1 )
        sfxlump = W_GetNumForName("dspistol");
    else
        sfxlump = W_GetNumForName(name);

    size = W_LumpLength( sfxlump );

    sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_STATIC );

    SetAudioStreamBufferSizeDefault( size );

    sfxsound->stream = LoadAudioStream( SFX_SAMPLE_RATE, SFX_BITS_PER_CHANNEL, SFX_CHANNELS);
    sfxsound->frameCount = size;

    printf( "%s %i %i\n", name, sfxlump, size );

    UpdateAudioStream(sfxsound->stream, sfx, size);

    return sfx;
}

void I_SetChannels()
{
}

 
void I_SetSfxVolume(d_uint volume)
{
    snd_SfxVolume = volume;
}

// MUSIC API - dummy. Some code from DOS version.
void I_SetMusicVolume(d_uint volume)
{
    // Internal state variable.
    snd_MusicVolume = volume;
    // Now set volume on output device.
    // Whatever( snd_MusciVolume );
}


//
// Retrieve the raw data lump index
//  for a given SFX name.
//
d_int I_GetSfxLumpNum(sfxinfo_t* sfx)
{
    char namebuf[9];

    snprintf(namebuf, 9, "ds%s", sfx->name);

    return W_GetNumForName(namebuf);
}

d_int I_StartSound (d_int id, d_int vol, d_int sep, d_int pitch, d_int priority)
{
    PlaySound( *sfx_audio_table[id] );

    return id;
}



void I_StopSound (d_int handle)
{
    StopSound( *sfx_audio_table[handle] );
}


d_int I_SoundIsPlaying(d_int handle)
{
    return IsSoundPlaying( *sfx_audio_table[handle] );
}

//
// This function loops all active (internal) sound
//  channels, retrieves a given number of samples
//  from the raw sound data, modifies it according
//  to the current (internal) channel parameters,
//  mixes the per channel samples into the global
//  mixbuffer, clamping it to the allowed range,
//  and sets up everything for transferring the
//  contents of the mixbuffer to the (two)
//  hardware channels (left and right, that is).
//
// This function currently supports only 16bit.
//
void I_UpdateSound( void )
{
}

// 
// This would be used to write out the mixbuffer
//  during each game loop update.
// Updates sound buffer and audio device at runtime. 
// It is called during Timer interrupt with SNDINTR.
// Mixing now done synchronous, and
//  only output be done asynchronous?
//
void I_SubmitSound(void)
{
}

void I_UpdateSoundParams( d_int handle, d_int vol, d_int sep, d_int pitch )
{
}

void I_ShutdownSound(void)
{
    if( IsAudioDeviceReady() )
        CloseAudioDevice();
}

void I_InitSound()
{
    InitAudioDevice();

    // Initialize external data (all sounds) at start, keep static.
    fprintf( stderr, "I_InitSound: ");

    for( d_int i = 1; i < NUMSFX; i++ )
    {
        // Alias? Example is the chaingun sound linked to pistol.
        if (!S_sfx[i].link)
        {
            // Load data from WAD file.
            S_sfx[i].data = getsfx( S_sfx[i].name, &sfx_audios[sfx_audio_amount] );
            sfx_audio_table[i] = &sfx_audios[sfx_audio_amount];
            sfx_audio_amount++;
        }
        else
        {
            // Previously loaded already?
            S_sfx[i].data = S_sfx[i].link->data;
            sfx_audio_table[i] = &sfx_audios[(S_sfx[i].link - S_sfx)/sizeof(sfxinfo_t)];
        }
    }
}

//
// MUSIC API.
// Still no music done.
// Remains. Dummies.
//
void I_InitMusic(void)
{
}

void I_ShutdownMusic(void)
{
}

void I_PlaySong(d_int handle, d_int looping)
{
}

void I_PauseSong (d_int handle)
{
}

void I_ResumeSong (d_int handle)
{
}

void I_StopSong(d_int handle)
{
}

void I_UnRegisterSong(d_int handle)
{
}

d_int I_RegisterSong(void* data)
{
  return 1;
}

// Is the song playing?
d_int I_QrySongPlaying(d_int handle)
{
  return false;
}


// Interrupt handler.
void I_HandleSoundTimer( d_int ignore )
{
  // TODO Add sound.
  // UNUSED, but required.
  ignore = 0;
  return;
}

// Get the interrupt. Set duration in millisecs.
d_int I_SoundSetTimer( d_int duration_of_tick )
{
  // TODO Add sound.
}


// Remove the interrupt. Set duration to zero.
void I_SoundDelTimer()
{
  // TODO Add sound.
}
