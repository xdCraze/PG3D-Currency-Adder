#pragma once
#include <stdexcept>
#include <format>

namespace IL2CPP
{
	namespace Exception
	{
		class PatternScanFailed : public ExceptionBase
		{
			public:
			PatternScanFailed() : ExceptionBase()
			{
				InitializeException(
					IL2CPP_NAMEOF(PatternScanFailed),
					"Couldn't find result with the specified pattern. Are sure the pattern is correct?"
				);
			}
		};
	}
}