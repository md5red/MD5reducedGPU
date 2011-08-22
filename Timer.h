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

#ifndef TIMER_H
 #define TIMER_H


#ifdef USE_BOOST_TIMER
#include <boost/date_time/posix_time/posix_time.hpp>
#else
#include <windows.h>
#endif

class CTimer
{
private:
#ifdef USE_BOOST_TIMER
	boost::posix_time::ptime m_ptStart;
	boost::posix_time::ptime m_ptEnd;
#else
	DWORD m_dwStart;
	DWORD m_dwEnd;
#endif

public:
	CTimer();
	~CTimer();

	void Start(); // Starts our timer.
	int Elapsed(); // Returns the elapsed time in ms.
	void Reset(); // Resets our timer.
};


#endif

