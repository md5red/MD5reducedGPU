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

#include "Main.h"
#include <iostream>
#include <cmath> // pow
#include <string>
#include "Precalc.h"
#include "MyGPU.h"

using namespace std;


// Constructor.
CPrecalc::CPrecalc()
{
	m_iSize = 0;
	m_iRealSize = 0;
	m_sPrecalc = NULL;
}

// Destructor.
CPrecalc::~CPrecalc()
{
	// Delete old data.
	SAFE_DELETE(m_sPrecalc);
}

// How many kernels do we need?
int CPrecalc::SplitInKernels(CCharset *forCharset)
{
	// Var.
	int kernels = 0;

	// Determine.
	switch (forCharset->GetLen())
	{
	case 10: // digits
		kernels = 1; // Slow, only 10k
		break;

	case 23: // special
		kernels = 1; // Slow, only 279k
		break;

	case 26: // small/caps
		kernels = 1; // Fast, 456k
		break;

	case 33: // digits & special
		kernels = 3; // Fast, 395k

	case 36: // small/caps & digits
		kernels = 2; // Fast, 839k
		break;

	case 49: // small/caps & spcial
		kernels = 7; // Fast, 823k.
		break;

	case 52: // small & caps
		kernels = 13; // Fast, 562k
		break;

	case 59: // small/caps & digits & special
		kernels = 59; // Slow, only 205k
		break;

	case 62: // small & caps & digits
		kernels = 31; // Fast, 476k
		break;

	case 85: // small & caps & special
		kernels = 85; // Fast, 614k
		break;

	case 95: // all
		kernels = 95; // Fast, 857k
		break;
#ifdef DEBUG
	default:
		cerr << "CPrecalc::SplitInKernels() failed -> No kernel config found for length " << forCharset->GetLen() << "!" << endl;
#endif
	}

	// Sucess!
	return kernels;
}

// Calculation.
bool CPrecalc::Calc(CPlain *forPlain, CCharset *forCharset, int forKernel, int maxKernels)
{
	// Split the work.
	int firstIndex = 0, lastIndex = 0;
	int charsetLen = (int)forCharset->GetLen();
	if (charsetLen > maxKernels) {
		firstIndex = (forKernel == 0) ? (int)((double)charsetLen / maxKernels * forKernel) : (int)((double)charsetLen / maxKernels * forKernel + 1);
		lastIndex = (int)((double)charsetLen / maxKernels * (forKernel + 1));
		if (lastIndex >= charsetLen)
			lastIndex = charsetLen - 1;
	}
	else if (charsetLen == maxKernels) {	
		firstIndex = forKernel;
		lastIndex = forKernel;
	}
	else {
#ifdef DEBUG
		cerr << "CPrecalc::Calc() failed -> Can't split the work, wrong pre-calc parameters found!" << endl;
#endif
		return false;
	}

	// Calc the array dimension.
	m_iSize = 1;
	for (int i = 1; i <= 3; i++)
		m_iSize *= charsetLen;
	m_iSize *= (lastIndex - firstIndex + 1);

	// Calc the real size.
	m_iRealSize = 1;
	for (int i = 1; i <= forPlain->GetLen(); i++)
		m_iRealSize *= charsetLen;
	m_iRealSize /= maxKernels;

	// Debug.
#ifdef EXTENDED_INFO
	cout << "[Debug] CPrecalc::Calc() -> forKernel = " << forKernel << endl;
	cout << "[Debug] CPrecalc::Calc() -> maxKernels = " << maxKernels << endl;
	cout << "[Debug] CPrecalc::Calc() -> firstIndex = " << firstIndex << endl;
	cout << "[Debug] CPrecalc::Calc() -> lastIndex = " << lastIndex << endl;
	cout << "[Debug] CPrecalc::m_iSize = " << m_iSize << endl;
	cout << "[Debug] CPrecalc::m_iRealSize = " << m_iRealSize << endl;
#endif

	// Safety first.
	if (m_iSize > (DIM_SIZE * DIM_SIZE)) {
#ifdef DEBUG
		cerr << "CPrecalc::Calc() failed -> m_iSize=" << m_iSize << " is too large" << endl;
#endif
		return false;
	}

	// Init a few things.
	SAFE_MALLOC(m_sPrecalc, Precalc_t, m_iSize);

	// The current index.
	int index = 0;

	// Compute.
	for (int i0 = firstIndex; i0 <= lastIndex; i0++) {
		for (int i1 = 0; i1 < charsetLen; i1++) {
			for (int i2 = 0; i2 < charsetLen; i2++) {
				for (int i3 = 0; i3 < charsetLen; i3++) {
					// Go.
					m_sPrecalc[index].i0 = i0;
					m_sPrecalc[index].i1 = i1;
					m_sPrecalc[index].i2 = i2;
					m_sPrecalc[index].i3 = i3;

					// Reset the saved state.
					m_sPrecalc[index].i4 = 0;
					m_sPrecalc[index].i5 = 0;
					m_sPrecalc[index].i6 = 0;
					m_sPrecalc[index].i7 = 0;
					m_sPrecalc[index].i8 = 0;
					m_sPrecalc[index].i9 = 0;
					m_sPrecalc[index].i10 = 0;

					// Bump index.
					index++;
				}
			}
		}
	}

	// Upload to device.
	if (!CMyGPU::GetInstance().MemcpyToSymbol("d_precalcSize", &m_iSize, sizeof(int))) {
#ifdef DEBUG
		cerr << "CPrecalc::Calc() failed -> Can't upload size of precalc to device with size " << m_iSize << endl;
#endif
		return false;
	}
	
	// Return.
	return (m_iSize == index);
}

Precalc_t *CPrecalc::GetPrecalc()
{
	return m_sPrecalc;
}

__int64 CPrecalc::GetSize()
{
	return m_iSize;
}

__int64 CPrecalc::GetRealSize()
{
	return m_iRealSize;
}