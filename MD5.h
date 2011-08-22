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

#ifndef MD5_H
#define MD5_H


#include <string>

class CMD5
{
private:
	std::string m_strHash;
	unsigned int m_iReversed[4];

public:
	CMD5();
	~CMD5();

	bool Initialize(std::string hash); // Inits our MD5 holder class.
	std::string GetHash(); // Returns the hash.

private:
	bool Check(); // Is it a vaild MD5 hash?
	void Reverse(); // Turn the MD5 hash back to the 4 state variables.

	bool UploadToDevice(); // Upload the 4 state variables to our device.
};


#endif