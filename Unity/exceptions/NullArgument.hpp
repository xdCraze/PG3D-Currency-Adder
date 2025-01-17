#pragma once
#include <stdexcept>
#include <format>

namespace IL2CPP
{
	namespace Exception
	{
		class NullArgument : public ExceptionBase
		{
			public:
			std::string paramname;

			explicit NullArgument(const std::string& paramname) : ExceptionBase()
			{
				this->paramname = paramname;

				InitializeException(
					IL2CPP_NAMEOF(NullArgument),
					std::format("\"{0}\" argument is null.", 
						paramname
					)
				);
			}
		};
	}
}