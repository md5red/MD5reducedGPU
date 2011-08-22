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

#ifndef CHARSET_H
 #define CHARSET_H


#include <string>
#include "Plain.h"

// The class.
class CCharset
{
private:
	std::string m_strCharset;

public:
	CCharset();
	~CCharset();

	bool Initialize(std::string chars);

	size_t GetLen(); // Returns the length of our charset.
	std::string GetCharset(); // Returns the whole charset.
	__int64 GetCombs(const int len); // Returns the possible combinations for this length.
	__int64 GetCombs(CPlain *forPlain); // See above, but taking our Plain class as parameter.
	__int64 GetAllCombs(const int min, const int max); // Return all possible combinations for length min -> max.

private:
	bool UploadToDevice(); // Upload the charset and the length to the device.
};


#endif
	