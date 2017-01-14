#pragma once
#include "Driver.h"
namespace sploit{namespace memory{


struct tagPhysStruct
{
	DWORD64 dwPhysMemSizeInBytes;
	PVOID pvPhysAddress;
	DWORD64 PhysicalMemoryHandle;
	DWORD64 pvPhysMemLin;
	DWORD64 pvPhysSection;
};

#define WINIO_IOCTL_INDEX 0x810
#define FILE_DEVICE_WINIO 0x00008010
#define IOCTL_WINIO_MAPPHYSTOLIN     CTL_CODE(FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX,      \
	METHOD_BUFFERED,        \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_UNMAPPHYSADDR    CTL_CODE(FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 1,  \
	METHOD_BUFFERED,        \
	FILE_ANY_ACCESS)



class Memory
{
private:
	Driver * m_Driver;

public:
	Memory(Driver * driver = 0); ~Memory();
	bool  ReadPhysMem(PVOID pbPhysAddr, PVOID pdwPhysVal, size_t size);
	bool  WritePhysMem(PVOID pbPhysAddr, PVOID dwPhysVal, size_t size);
private:
	PVOID  MapPhysToLin(tagPhysStruct &PhysStruct);
	bool  UnmapPhysicalMemory(tagPhysStruct &PhysStruct);

};


}}
