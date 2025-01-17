#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	namespace DefaultTypeClass
	{
		extern const Class* Type;
		extern const Class* Boolean;
		extern const Class* Byte;
		extern const Class* Int16;
		extern const Class* Int32;
		extern const Class* Int64;
		extern const Class* Float;
		extern const Class* Double;
		extern const Class* Decimal;
		extern const Class* Char;
		extern const Class* String;
		extern const Class* Object;
		extern const Class* Array;
	}

	namespace DefaultImage
	{
		extern const Image* Corlib;
		extern const Image* System;
		extern const Image* UnityCoreModule;
		extern const Image* UnityPhysicsModule;
		extern const Image* AssemblyCSharp;
		extern const Image* InputLegacyModule;
		extern const Image* Photon3Unity3D;
		extern const Image* SteamworksNet;
		extern const Image* UserSessionManagement;
	}

	namespace CommonCShrap
	{
		void INIT();
	}
}
