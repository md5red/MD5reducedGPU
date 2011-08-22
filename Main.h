/*
MD5reducedGPU
Copyright (C) 2011 Tobias Sielaff

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAIN_H
 #define MAIN_H


// General
#define VERSION "1.0.0"

// Build config.
#define DEBUG // Enables a few more error prints/mem leak tests.
//#define EXTENDED_INFO // Prints calculated values.
#define USE_BOOST_TIMER // Boost doesn't compile with nvcc, so we resort to GetTickCount().

// Kernel config.
// This needs Compute Capability > 2.0!
#define DIM_SIZE 1024

// This is the Kernel work-load.
// Set this to high and the pc might become
// unresponsive, set it to low and the speed
// will become laugable.
#define WORK_LOAD 300

// Helpers.
#define SAFE_DELETE(ptr)	\
	if (ptr != NULL)		\
		delete[] ptr;		\
	ptr = NULL;

#define SAFE_MALLOC(ptr, type, size)	\
	SAFE_DELETE(ptr)					\
	ptr = new type[size];

// Struct
// Graphic cards usually work better on 32 bit datatypes,
// but this is some kind of trade-off, because transfering
// 12 x 32 bit takes much longer than 12 x 8 bit.
typedef struct Precalc_s
{
	// These are our 4 pre-calculated
	// starting points.
	char i0;
	char i1;
	char i2;
	char i3;

	// These are our 7 variables to
	// save the state of the for-loops
	// inside the kernel.
	char i4;
	char i5;
	char i6;
	char i7;
	char i8;
	char i9;
	char i10;
} Precalc_t;


#endif