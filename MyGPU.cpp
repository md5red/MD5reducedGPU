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
#include <cutil.h>
#include <cutil_inline.h>
#include <boost/format.hpp>
#include "Main.h"
#include "MyGPU.h"

using namespace std;


// Constructor.
CMyGPU::CMyGPU()
{
#ifdef DEBUG
	m_iAlloced = 0;
#endif
}

// Destructor.
CMyGPU::~CMyGPU()
{
#ifdef DEBUG
	if (m_iAlloced != 0)
		cerr << "CMyGPU::CMyGPU() -> Memory leak detected! Remaining allocations: " << m_iAlloced << endl;
#endif
}

// Singleton.
CMyGPU &CMyGPU::GetInstance()
{
	static CMyGPU theInstance;
	return theInstance;
}

// Init.
bool CMyGPU::Initialize()
{
	// Get device.
	int device = cutGetMaxGflopsDeviceId();

	// See if we have compute capability 2.x
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, device);
	if (deviceProp.major < 2) {
#ifdef DEBUG
		cerr << "CMyGPU::Initialize() failed -> CUDA Capability is not 2.x!" << endl;
#endif
		return false;
	}

	// Set device.
	cudaError_t err = cudaSetDevice(device);

	// Check
#ifdef DEBUG
	if (err != cudaSuccess)
		cerr << "CMyGPU::Initialize() failed -> Return code: " << err << endl;
#endif

	// Return.
	return (err == cudaSuccess);	
}

string CMyGPU::GetDeviceName()
{
	// Get device.
	int device = cutGetMaxGflopsDeviceId();

	// Get device probs.
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, device);

	// Return the name.
	return deviceProp.name;
}

string CMyGPU::GetCudaCapability()
{
	// Get device.
	int device = cutGetMaxGflopsDeviceId();

	// Get device probs.
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, device);

	// Return the name.
	return str(boost::format("%1%.%2%") % deviceProp.major % deviceProp.minor);
}

// Malloc.
bool CMyGPU::Malloc(void **ptr, size_t size)
{
	cudaError_t err = cudaMalloc(ptr, size);

#ifdef DEBUG
	if (err == cudaSuccess)
		m_iAlloced++;
	else
		cerr << "CMyGPU::Malloc() failed -> Return code: " << err << endl;
#endif

	return (err == cudaSuccess);
}

// Free.
bool CMyGPU::Free(void *ptr)
{
	// Free it.
	cudaError_t err = cudaFree(ptr);
	
#ifdef DEBUG
	if (err == cudaSuccess)
		m_iAlloced--;
	else
		cerr << "[Debug] CMyGPU::Free() failed -> Return code: " << err << endl;
#endif

	// Null it.
	ptr = NULL;

	// Return.
	return (err == cudaSuccess);
}

// Memcpy
bool CMyGPU::MemcpyToDevice(void* ptr, const void* src, size_t size)
{
	cudaError_t err = cudaMemcpy(ptr, src, size, cudaMemcpyHostToDevice);
#ifdef DEBUG
	if (err != cudaSuccess)
		cerr << "[Debug] CMyGPU::MemcpyToDevice() failed -> Return code: " << err << endl;
#endif
	return (err == cudaSuccess);
}

bool CMyGPU::MemcpyToHost(void* ptr, const void* src, size_t size)
{
	cudaError_t err = cudaMemcpy(ptr, src, size, cudaMemcpyDeviceToHost);
#ifdef DEBUG
	if (err != cudaSuccess)
		cerr << "[Debug] CMyGPU::MemcpyToHost() failed -> Return code: " << err << endl;
#endif
	return (err == cudaSuccess);
}

// MemcpyToSymbol
bool CMyGPU::MemcpyToSymbol(const char *symbol, const void *src, size_t size)
{
	cudaError_t err = cudaMemcpyToSymbol(symbol, src, size);
#ifdef DEBUG
	if (err != cudaSuccess)
		cerr << "[Debug] CMyGPU::MemcpyToSymbol() failed -> Return code: " << err << endl;
#endif
	return (err == cudaSuccess);
}