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

#ifndef PRECALC_H
 #define PRECALC_H


#include "Main.h"
#include "Plain.h"
#include "Charset.h"

class CPrecalc
{
private:
	Precalc_t *m_sPrecalc;
	__int64 m_iSize;
	__int64 m_iRealSize;

public:
	CPrecalc();
	~CPrecalc();

	int SplitInKernels(CCharset *forCharset); // Precalculation might be too large to handle in a single kernel, therefor we are splitting it.
	bool Calc(CPlain *forPlain, CCharset *forCharset, int forKernel, int maxKernels); // Do the acutal precalculation. This is always 4 rounds.
	
	Precalc_t *GetPrecalc(); // Returns the pointer to our precalc.
	__int64 GetSize(); // Return the array boundaries.
	__int64 GetRealSize(); // Return all possible combinations for the precalculation.
};


#endif