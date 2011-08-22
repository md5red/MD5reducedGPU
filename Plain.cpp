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
#include "Plain.h"
#include "MyGPU.h"

using namespace std;


CPlain::CPlain(int len)
	: m_iLen(len)
{
	// Upload it.
	UploadToDevice();
}

CPlain::~CPlain()
{
}

int CPlain::GetLen()
{
	return m_iLen;
}

void CPlain::UploadToDevice()
{
	// Upload to device.
	if (!CMyGPU::GetInstance().MemcpyToSymbol("d_plainLen", &m_iLen, sizeof(int))) {
#ifdef DEBUG
		cerr << "CPlain::UploadToDevice() failed -> Can't upload length of plain to device with size " << m_iLen << endl;
#endif
		return;
	}
}