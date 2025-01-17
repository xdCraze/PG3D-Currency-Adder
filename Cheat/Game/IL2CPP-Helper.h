#include "Patterns.h">
#include <MinHook.h>
#include "../Utils/Logger.h"

// Define Hook
void Hook(LPVOID pointer, LPVOID detour, LPVOID* original)
{
    if (MH_CreateHook(pointer, detour, original) == MH_OK)
    {
        MH_EnableHook(pointer);
    }
    else
        Logger::Log("Failed to hook offset: 0x{}", Logger::LogLevel::ERRORR, pointer);
}

void HookMethod(IL2CPP::MethodInfo* method, LPVOID detour, LPVOID* original)
{
    Hook(method->GetPointer(), detour, original);
}

void HookMethodViaPattern(IL2CPP::Class* klass, IL2CPP::SignaturePattern* pattern, LPVOID detour, LPVOID* original)
{
    Hook(klass->GetMethodByPattern(*pattern)->GetPointer(), detour, original);
}

struct AnalyticsParams {
	int field1;
	int field2;
	int field3;
	int field4;
	int field5;
	int field6;
	int field7;
	bool field8;
	int field9;
	int field10;
	bool field11;
	int field12;
	int field13;
	bool field14;
	int field15;
	int field16;
	int field17;
	bool field18;
	int64_t field19;
	int field20;
	bool field21;
	bool field22;
	bool field23;
	bool field24;
	int field25;
};

AnalyticsParams offerwallParams = {
	0,
	0x18,
	0x82,
	0,
	0,
	0,
	0,
	false,
	0,
	0,
	false,
	0,
	0,
	false,
	0,
	0,
	0,
	false,
	0,
	0,
	false,
	false,
	false,
	true,
	0
};