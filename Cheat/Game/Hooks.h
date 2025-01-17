// Including
#include "IL2CPP-Helper.h"

std::string progressUpdate_keyname = "ProgressUpdater"; // You can put whatever you want here it doesnt really matter

// Pointers
IL2CPP::Wrapper::Method<IL2CPP::Object*()> PUInstance;
IL2CPP::Wrapper::Method<void(IL2CPP::Object* Instance, IL2CPP::String* CurrencyName, int Amount, int Cause, bool, bool, AnalyticsParams)> AddCurrency; // Params are necessary :)


void Pointers()
{
	IL2CPP::Class* progressUpdater_class = IL2CPP::ClassMapping::GetClass(progressUpdate_keyname);
	AddCurrency = progressUpdater_class->GetMethod(0x91);
	PUInstance = progressUpdater_class->GetMethod(0x58);
}

void(*oldPixelTime)(IL2CPP::Object* Instance);
void PixelTime(IL2CPP::Object* Instance)
{
	if (Instance != nullptr)
	{
		if (RunBackend.AddCurrency)
		{
			AddCurrency(PUInstance(), IL2CPP::String::Create(RunBackend.CurrencyList[RunBackend.CurrencyType]), RunBackend.CurrencyAmount, 1, true, true, offerwallParams);
			RunBackend.AddCurrency = false;
		}
	}

	return oldPixelTime(Instance);
}

namespace FieldPatterns {
	IL2CPP::FieldPattern progressUpdaterPattern{ "List`1", "List`1", "String", "String", "String", nullptr, "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "Action", "Action", "Action", "Action", "Boolean", "Boolean", "String", "String", "Queue`1", "List`1", "List`1", "Dictionary`2", "Dictionary`2", nullptr, "String", "Boolean", "Int32", "String", "Boolean", "String", "Boolean", "String", "Action`1", "Action`1", "Dictionary`2", "List`1", "Dictionary`2", "Dictionary`2", "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "Dictionary`2", "List`1", "List`1", "Int32", "Int32", "Dictionary`2", "List`1", "Coroutine", "UInt32" };
}

// Hooking
void Hooks()
{
	Logger::Log("Hooking...", Logger::LogLevel::INFO);

	// You need these
	IL2CPP::Domain* Domain = IL2CPP::CurrentDomain();
	IL2CPP::Image* Assembly = Domain->OpenAssembly("Assembly-CSharp.dll");

	// Classes
	IL2CPP::Class* playerMoveC_class = Assembly->GetClass("Player_move_c");
	IL2CPP::Class* weaponSounds_class = Assembly->GetClass("WeaponSounds");
	IL2CPP::Class* PixelTime_class = Assembly->GetClass("PixelTime");
	IL2CPP::Class* experienceController_class = Assembly->GetClass("ExperienceController");

	// Hook PixelTime
	HookMethod(PixelTime_class->GetMethod("Update"), PixelTime, (void**)&oldPixelTime);

	// Hooking ProgressUpdater (you do it this way because its obfuscated)
	IL2CPP::ClassMapping::AddImageToScan(Assembly);
	IL2CPP::ClassMapping::AddQueue(progressUpdate_keyname, "Progress", &FieldPatterns::progressUpdaterPattern);
	IL2CPP::ClassMapping::StartMapping();

    // Pointer Usage
    Pointers();

	Logger::Log("Successfully hooked!\n", Logger::LogLevel::INFO);
}