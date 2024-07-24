#include "urmem.hpp"
#include "BitStream.h"

#ifdef _WIN32
#define CALL __stdcall
#define EXTERN extern "C"

#define TC_FUNCDEF(name, ...) \
	__fastcall name(RakNet::BitStream* _this, void* edx, __VA_ARGS__)

#define BSRead_PATTERN "\x8B\xC1\x8B\x50\x08\xF6\xC2\x07\x8B\x4C\x24\x08\x75\x47\x55\x56\x8D\x2C\xCD\x00\x00\x00\x00\x57\x8B\x38\x8D\x34\x2A\x3B\xF7", "xxxxxxxxxxxxxxxxxxx????xxxxxxxx"
#define BSReadBits_PATTERN "\x8B\x54\x24\x08\x85\xD2\x56\x8B\xF1\x7E\x0B\x8B\x46\x08\x8B\x0E\x03\xC2\x3B\xC1\x7E\x06", "xxxxxxxxxxxxxxxxxxxxx"

#define BSReadBool_PATTERN "\x8B\xC1\x8B\x48\x08\x8B\x10\x56\x8D\x71\x01\x3B\xF2\x7E\x06\x32\xC0\x5E\xC2\x04\x00\x8B\xD1", "xxxxxxxxxxxxxxxxxxxxxxx"

#elif __linux__
#define CALL
#define EXTERN extern "C" __attribute__((visibility("default")))

#define TC_FUNCDEF(name, ...) \
	name(RakNet::BitStream* _this, __VA_ARGS__)

#define BSRead_PATTERN "\x55\x89\xE5\x83\xEC\x28\x89\x5D\xF4\x8B\x5D\x08\x89\x75\xF8\x8B\x75\x10\x89\x7D\xFC\x8B\x53\x08\xF6\xC2\x07\x75\x55\x8D\x3C\xF5\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????"
#define BSReadBits_PATTERN "\x55\x31\xD2\x89\xE5\x57\x56\x53\x83\xEC\x1C\x8B\x75\x10\x0F\xB6\x45\x14\x8B\x7D\x08\x85\xF6\x8B\x5D\x0C", "xxxxxxxxxxxxxxxxxxxxxxxxxx"

#endif

#define HookFunctionWithPattern(name)                                  \
	urmem::address_t name##_FuncAddr;                                  \
	if (!scanner.find(name##_PATTERN, name##_FuncAddr))                \
	{                                                                  \
		return false;                                                  \
	}                                                                  \
	hook##name.install(name##_FuncAddr, urmem::get_func_addr(name)); \
	hook##name.enable()

typedef void(*logprintf_t)(const char* format, ...);
logprintf_t logprintf;

urmem::hook hookBSReadBool;
urmem::hook hookBSRead;
urmem::hook hookBSReadBits;

inline int GetNumberOfUnreadBits(RakNet::BitStream* bs)
{
	return bs->GetReadOffset() > bs->GetNumberOfBitsUsed() ? 0 : bs->GetNumberOfBitsUsed() - bs->GetReadOffset();
}

bool TC_FUNCDEF(BSRead, char* output, int numberOfBytes)
{
	// logprintf("BitStream::Read: numberOfBytes = %d, GetNumberOfUnreadBits = %d", numberOfBytes, GetNumberOfUnreadBits(_this));
	if ((_this->GetReadOffset() & 7) == 0 && _this->GetNumberOfUnreadBits() < (numberOfBytes << 3))
	{
		return false;
	}

	return hookBSRead.call<urmem::calling_convention::thiscall, bool>(_this, output, numberOfBytes);
}

bool TC_FUNCDEF(BSReadBits, char* output, int numberOfBitsToRead, bool alignBitsToRight)
{
	// logprintf("BitStream::ReadBits: numberOfBitsToRead = %d, GetNumberOfUnreadBits = %d", numberOfBitsToRead, GetNumberOfUnreadBits(_this));
	if (numberOfBitsToRead <= 0)
	{
		return false;
	}

	if (GetNumberOfUnreadBits(_this) < numberOfBitsToRead)
	{
		return false;
	}

	return hookBSReadBits.call<urmem::calling_convention::thiscall, bool>(_this, output, numberOfBitsToRead, alignBitsToRight);
}

bool TC_FUNCDEF(BSReadBool, bool* value)
{
	if (_this->GetNumberOfUnreadBits() == 0)
	{
		return false;
	}

	return hookBSReadBool.call<urmem::calling_convention::thiscall, bool>(_this, value);
}

EXTERN bool CALL Load(void** ppData)
{
	logprintf = (logprintf_t)(ppData[0]);

	urmem::sig_scanner scanner;
	scanner.init(reinterpret_cast<urmem::address_t>(ppData[0]));

	HookFunctionWithPattern(BSRead);
	HookFunctionWithPattern(BSReadBits);

#ifdef WIN32
	HookFunctionWithPattern(BSReadBool);
#endif

	logprintf("\tBitStream Crash Fix plugin by iAmir has been successfully loaded");
	logprintf("\t==== https://github.com/AmyrAhmady/samp-bscrashfix");
	return true;
}

EXTERN unsigned int CALL Supports() { return 0x0200 | 0x10000; };