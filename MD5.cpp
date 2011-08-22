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
#include <sstream>
#include <algorithm> // transform
#include <iomanip> // setprecision
#include "MD5.h"
#include "MyGPU.h"

using namespace std;


// Constructor
CMD5::CMD5()
{
}

// Destructor
CMD5::~CMD5()
{
}

// Init.
bool CMD5::Initialize(string hash)
{
	// Save hash.
	m_strHash = hash;

	// Check.
	if (!Check())
		return false;

	// Reverse.
	Reverse();

	// Upload.
	return UploadToDevice();
}

bool CMD5::UploadToDevice()
{
	if (!CMyGPU::GetInstance().MemcpyToSymbol("d_state", &m_iReversed, sizeof(m_iReversed))) {
#ifdef DEBUG
		cerr << "CMD5::UploadToDevice() failed -> Can't upload reversed state to device." << endl;
#endif
		return false;
	}

	// Success!
	return true;
}

bool CMD5::Check()
{
	// Check length.
	if (m_strHash.size() != 32) {
#ifdef DEBUG
		cerr << "CMD5::Check() failed -> Hash is not 32 chars long." << endl;
#endif
		return false;
	}

	// Turn to lower.
	transform(m_strHash.begin(), m_strHash.end(), m_strHash.begin(), tolower);

	// Check if hex.
	for (int i = 0; i < m_strHash.size(); i++)
		if (!isxdigit(m_strHash[i])) {
#ifdef DEBUG
			cerr << "CMD5::Check() failed -> Hash not consiting of only hex chars." << endl;
#endif
			return false;
		}

	// Fine!
	return true;
}

void CMD5::Reverse()
{
	// Temporary var.
	unsigned char digest[16];

	// Turn hex encoded string back into bytes.
	for (unsigned int i = 0; i < m_strHash.size() / 2; i++) {
		int val = 0;
		stringstream ss;
		ss << m_strHash.substr(i * 2, 2);
		ss >> setprecision(2) >> hex >> val;
		digest[i] = (unsigned char)val;
	}

	// Copy everything to our state array.
	memcpy(m_iReversed, digest, 16);

	// Substract the magic.
	m_iReversed[0] -= 0x67452301;
	m_iReversed[1] -= 0xefcdab89;
	m_iReversed[2] -= 0x98badcfe;
	m_iReversed[3] -= 0x10325476;
}

string CMD5::GetHash()
{
	return m_strHash;
}