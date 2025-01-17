#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct Object : private IMPORT::Il2CppObject
	{
		Class* GetClass() const;

		void* GetFieldPtr(const std::string& fieldName);

		void* GetFieldPtr(size_t index);

		template<typename T>
		inline T& GetFieldRef(const std::string& fieldName)
		{
			return *(T*) GetFieldPtr(fieldName);
		}

		template<typename T>
		inline T& GetFieldRef(size_t index)
		{
			return *(T*) GetFieldPtr(index);
		}

		template<typename T>
		inline T Unbox()
		{
			return *(T*)IMPORT::il2cpp_object_unbox(this);
		}

		template<typename T>
		static Object* BoxValue(Class* klass, T* value)
		{
			if (klass == nullptr)
			{
				throw Exception::NullArgument(
					IL2CPP_NAMEOF(klass)
				);
			}

			return (Object*) IMPORT::il2cpp_value_box(
				(IMPORT::Il2CppClass*) klass, 
				value
			);
		}
	};
}