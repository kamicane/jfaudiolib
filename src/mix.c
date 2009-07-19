/*
 Copyright (C) 2009 Jonathon Fowler <jf@jonof.id.au>
 
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

#include "_multivc.h"

extern char  *MV_HarshClipTable;
extern char  *MV_MixDestination;			// pointer to the next output sample
extern unsigned int MV_MixPosition;		// return value of where the source pointer got to
extern short *MV_LeftVolume;
extern short *MV_RightVolume;
extern int    MV_SampleSize;
extern int    MV_RightChannelOffset;



void ClearBuffer_DW( void *ptr, unsigned data, int length )
{
	unsigned *ptrdw = ptr;
	while (length--) {
		*(ptrdw++) = data;
	}
}

/*
 JBF:
 
 position = offset of starting sample in start
 rate = resampling increment
 start = sound data
 length = count of samples to mix
 */

// 8-bit mono source, 8-bit mono output
void MV_Mix8BitMono( unsigned int position, unsigned int rate,
							char *start, unsigned int length )
{
	unsigned char *source = (unsigned char *) start;
	unsigned char *dest = (unsigned char *) MV_MixDestination;
	int sample0;
	
	length &= ~1;
	
	while (length--) {
		sample0 = source[position >> 16];
		position += rate;
		
		sample0 = MV_LeftVolume[sample0] + *dest;
		sample0 = MV_HarshClipTable[sample0 + 128];
		
		*dest = sample0 & 255;
		
		dest += MV_SampleSize;
	}
	
	MV_MixPosition = position;
	MV_MixDestination = (char *) dest;
}

// 8-bit mono source, 8-bit stereo output
void MV_Mix8BitStereo( unsigned int position, unsigned int rate,
							  char *start, unsigned int length )
{
	unsigned char *source = (unsigned char *) start;
	unsigned char *dest = (unsigned char *) MV_MixDestination;
	int sample0, sample1;
	
	length &= ~1;
	
	while (length--) {
		sample0 = source[position >> 16];
		sample1 = sample0;
		position += rate;
		
		sample0 = MV_LeftVolume[sample0] + *dest;
		sample1 = MV_RightVolume[sample1] + *(dest + MV_RightChannelOffset);
		sample0 = MV_HarshClipTable[sample0 + 128];
		sample1 = MV_HarshClipTable[sample1 + 128];
		
		*dest = sample0 & 255;
		*(dest + MV_RightChannelOffset) = sample1 & 255;
		
		dest += MV_SampleSize;
	}
	
	MV_MixPosition = position;
	MV_MixDestination = (char *) dest;
}

// 8-bit mono source, 16-bit mono output
void MV_Mix16BitMono( unsigned int position, unsigned int rate,
							 char *start, unsigned int length )
{
	unsigned char *source = (unsigned char *) start;
	short *dest = (short *) MV_MixDestination;
	int sample0;
	
	length &= ~1;
	
	while (length--) {
		sample0 = source[position >> 16];
		position += rate;
		
		sample0 = MV_LeftVolume[sample0] + *dest;
		if (sample0 < -32768) sample0 = -32768;
		else if (sample0 > 32767) sample0 = 32767;
		
		*dest = (short) sample0;
		
		dest += MV_SampleSize / 2;
	}
	
	MV_MixPosition = position;
	MV_MixDestination = (char *) dest;
}

// 8-bit mono source, 16-bit stereo output
void MV_Mix16BitStereo( unsigned int position, unsigned int rate,
								char *start, unsigned int length )
{
	unsigned char *source = (unsigned char *) start;
	short *dest = (short *) MV_MixDestination;
	int sample0, sample1;
	
	while (length--) {
		sample0 = source[position >> 16];
		sample1 = sample0;
		position += rate;
		
		sample0 = MV_LeftVolume[sample0] + *dest;
		sample1 = MV_RightVolume[sample1] + *(dest + MV_RightChannelOffset / 2);
		if (sample0 < -32768) sample0 = -32768;
		else if (sample0 > 32767) sample0 = 32767;
		if (sample1 < -32768) sample1 = -32768;
		else if (sample1 > 32767) sample1 = 32767;
		
		*dest = (short) sample0;
		*(dest + MV_RightChannelOffset/2) = (short) sample1;
		
		dest += MV_SampleSize / 2;
	}
	
	MV_MixPosition = position;
	MV_MixDestination = (char *) dest;
}

// 16-bit mono source, 16-bit mono output
void MV_Mix16BitMono16( unsigned int position, unsigned int rate,
								char *start, unsigned int length )
{
}

// 16-bit mono source, 8-bit mono output
void MV_Mix8BitMono16( unsigned int position, unsigned int rate,
							  char *start, unsigned int length )
{
}

// 16-bit mono source, 8-bit stereo output
void MV_Mix8BitStereo16( unsigned int position, unsigned int rate,
								 char *start, unsigned int length )
{
}

// 16-bit mono source, 16-bit stereo output
void MV_Mix16BitStereo16( unsigned int position, unsigned int rate,
								  char *start, unsigned int length )
{
}

void MV_16BitReverb( char *src, char *dest, VOLUME16 *volume, int count )
{
}

void MV_8BitReverb( signed char *src, signed char *dest, VOLUME16 *volume, int count )
{
}

void MV_16BitReverbFast( char *src, char *dest, int count, int shift )
{
}

void MV_8BitReverbFast( signed char *src, signed char *dest, int count, int shift )
{
}
