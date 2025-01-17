#pragma once
#include <stdexcept>
#include <format>

namespace IL2CPP
{
	namespace Exception
	{
		class UnexpectedError : public ExceptionBase
		{
			public:
			UnexpectedError() : ExceptionBase()
			{
				InitializeException(
					IL2CPP_NAMEOF(UnexpectedError),
					"Unexpected error occurred during runtime."
				);
			}
		};
	}
}