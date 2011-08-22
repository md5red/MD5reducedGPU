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

#ifndef MYGPU_H
 #define MYGPU_H


#include <string>

class CMyGPU
{
#ifdef DEBUG
private:
	int m_iAlloced; // Searching for Mem-Leaks on GPU.
#endif

public:
	static CMyGPU &CMyGPU::GetInstance(); // Singleton.

	bool Initialize(); // Init our graphic card. Always choses the fastest.

	std::string GetDeviceName(); // Returns the name of the device.
	std::string GetCudaCapability(); // Return the cuda capability.

	bool Malloc(void** ptr, size_t size); // Alloc some space on the GPU.
	bool Free(void* ptr); // Cleanup on GPU.
	bool MemcpyToDevice(void* ptr, const void* src, size_t size); // Memcpy: Host -> Device.
	bool MemcpyToHost(void* ptr, const void* src, size_t size); // Memcpy: Device -> Host.
	bool MemcpyToSymbol(const char *symbol, const void *src, size_t size); // Memcpy: Host -> Device (constant memory).

protected:
	CMyGPU();
	~CMyGPU();
};


#endif