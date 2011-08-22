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
#include "Charset.h"
#include "MyGPU.h"

using namespace std;

// Charset blocks.
const char h_charset_num[] = "0123456789";
const char h_charset_alpha_l[] = "abcdefghijklmnopqrstuvwxyz";
const char h_charset_alpha_h[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char h_charset_printable[] = " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";


// Constructor.
CCharset::CCharset()
{
}

// Destructor.
CCharset::~CCharset()
{
}

// Init.
bool CCharset::Initialize(string chars)
{
	// Length plausible?
	if (chars.size() > 4)
		return false;

	// Fill.
	// TODO: What do we do with ccc?
	string charset;
	for (size_t i = 0; i < chars.size(); i++) {
		if (chars[i] == 'c')
			charset += h_charset_alpha_h;

		if (chars[i] == 's')
			charset += h_charset_alpha_l;

		if (chars[i] == 'd')
			charset += h_charset_num;

		if (chars[i] == 'x')
			charset += h_charset_printable;
	}

	// Save.
	m_strCharset = charset;

	// Upload.
	return UploadToDevice();
}

size_t CCharset::GetLen()
{
	return m_strCharset.size();
}

string CCharset::GetCharset()
{
	return m_strCharset;
}

bool CCharset::UploadToDevice()
{
	// Size.
	int size = static_cast<int>(m_strCharset.size());

	// Copy charset to device.
	if (!CMyGPU::GetInstance().MemcpyToSymbol("d_charset", m_strCharset.c_str(), m_strCharset.size())) {
#ifdef DEBUG
		cerr << "CCharset::UploadToDevice() failed -> Can't upload charset to device with size " << size << endl;
#endif
		return false;
	}

	// Copy length of charset to device.
	if (!CMyGPU::GetInstance().MemcpyToSymbol("d_charsetLen", &size, sizeof(int))) {
#ifdef DEBUG
		cerr << "CCharset::UploadToDevice() failed -> Can't upload length of charset to device with size " << size << endl;
#endif
		return false;
	}

	// Success!
	return true;
}

__int64 CCharset::GetCombs(const int len)
{
	// <cmath>'s pow just won't cut it for the 64 bit datatypes.
	__int64 combs = 1;
	for (int i = 0; i < len; i++)
		combs *= m_strCharset.size();
	return combs;
}

__int64 CCharset::GetCombs(CPlain *forPlain)
{
	return GetCombs(forPlain->GetLen());
}

__int64 CCharset::GetAllCombs(const int min, const int max)
{
	__int64 combs = 0;
	for (int i = min; i <= max; i++)
		combs += GetCombs(i);

	return combs;
}