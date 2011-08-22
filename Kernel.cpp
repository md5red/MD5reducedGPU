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
#include <string>
#include "Kernel.h"
#include "Timer.h"
#include "MD5.cuh"

using namespace std;


CKernel::CKernel()
{
	m_iSumComputed = 0;
	m_iCurComputed = 0;
	m_iRunTime = 0;

	m_cResult = NULL;
}

CKernel::~CKernel()
{
	// Cleanup.
	SAFE_DELETE(m_cResult);
}

bool CKernel::Execute(CPlain *forPlain, CCharset *forCharset, CPrecalc *forPrecalc)
{
	// Make room for the result.
	SAFE_MALLOC(m_cResult, char, forPlain->GetLen() + 1);

	// Null termination (strlen, cout, ... etc. need it).
	m_cResult[forPlain->GetLen()] = '\0';

	// Go.
	CTimer myTimer;
	myTimer.Start();
	if (!ExecuteKernel(forPlain, forCharset, forPrecalc, m_cResult))
		return false;

	// Update stats.
	m_iRunTime = myTimer.Elapsed();
	m_iCurComputed = forPrecalc->GetRealSize();
	m_iSumComputed += m_iCurComputed;
	
	// Debug.
#ifdef EXTENDED_INFO
	cout << "[Debug] CKernel::m_iRunTime  =" << m_iRunTime << endl;
	cout << "[Debug] CKernel::m_iCurComputed = " << m_iCurComputed << endl;
	cout << "[Debug] CKernel::m_iSumComputed = " << m_iSumComputed << endl;
	cout << "[Debug] CKernel::GetProgress() = " << GetProgress(forPlain, forCharset) << endl;
	cout << "[Debug] CKernel::GetSpeed() = " << GetSpeed() << endl;
#endif

	// Return.
	return (strlen(m_cResult) > 0);
}

char* CKernel::GetResult()
{
	return m_cResult;
}

double CKernel::GetProgress(CPlain *forPlain, CCharset *forCharset)
{
	// Compute.
	return (m_iSumComputed / static_cast<long double>(forCharset->GetCombs(forPlain)) * 100);
}

double CKernel::GetSpeed()
{
	// This might happen if the kernel finishes so
	// fast we couldn't meassure the time.
	if (m_iRunTime == 0)
		m_iRunTime = 1;

	// Compute.
	double hashPerMs = static_cast<long double>(m_iCurComputed) / m_iRunTime;
	double hashPerS = hashPerMs * 1000;
	double MHashPerS = hashPerS / 1000000;

	// Return.
	return MHashPerS;
}