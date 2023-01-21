#include <stdio.h>
#include <string.h>
#include <sys\time.h>
#include <pthread.h>
#include <utility>
#include "natives.h"
#include <sys\time.h>
#pragma clang diagnostic ignored "-Wunknown-pragmas"

void PatchInJump(uint64_t address, const void* destination)
{
	if (!address || !destination)
	*(uint8_t*)(address) = 0xFF;
	*(uint8_t*)(address + 1) = 0x25;
	*(uint8_t*)(address + 2) = 0x00;
	*(uint8_t*)(address + 3) = 0x00;
	*(uint8_t*)(address + 4) = 0x00;
	*(uint8_t*)(address + 5) = 0x00;
	*(uint64_t*)(address + 6) = (uint64_t)destination;
}

#pragma region InGame
void printSubtitle(const char* text)
{
	const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", text);
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(literalString);
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}
#pragma endregion

void* main_thread(void*)
{
	printSubtitle("Hello World\n");
	scePthreadExit(0);
}

ScePthread threadid;
extern "C"
{
	int module_start(size_t args, const void* argp)
	{
		scePthreadSetcancelstate(PTHREAD_CANCEL_ENABLE, 0);
		scePthreadCreate(&threadid, NULL, main_thread, NULL, "RDR2");
		return 0;
	}

	int module_stop(size_t args, const void* argp)
	{
		scePthreadCancel(threadid);
		scePthreadExit(0);
		return 0;
	}
}
#pragma clang diagnostic pop