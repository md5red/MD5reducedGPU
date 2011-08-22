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

#ifndef KERNEL_H
 #define KERNEL_H


#include "Plain.h"
#include "Charset.h"
#include "Precalc.h"

class CKernel
{
private:
	__int64 m_iSumComputed;
	__int64 m_iCurComputed;

	int m_iRunTime;
	char* m_cResult;

public:
	CKernel();
	~CKernel();

	bool Execute(CPlain *forPlain, CCharset *forCharset, CPrecalc *forPrecalc); // Start our Kernel.

	char* GetResult(); // Returns a result.
	double GetProgress(CPlain *forPlain, CCharset *forCharset); // Returns the progress for the current plain length.
	double GetSpeed(); // Return the speed. This is calulated after every sucessfull Kernel execution.
};


#endif