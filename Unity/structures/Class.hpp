#pragma once
#include "../Main.hpp"
#include <vector>

namespace IL2CPP
{
	struct Class : private IMPORT::Il2CppClass
	{
		// WARNING: You must call class constructor to fully define the object.
		static Object* Create(Class* klass);

		void ForceInitializeClass() const;

		Class* GetParentClass() const;

		size_t InterfaceCount() const;

		Class* GetInterface(size_t index) const;

		std::string GetNamespace() const;

		std::string GetName() const;

		std::string GetFullName() const;

		uint16_t FieldCount() const;

		FieldInfo* GetField(size_t index) const;

		FieldInfo* GetField(const std::string& fieldName) const;

		bool HasFields() const;

		bool IsFieldsInitialized() const;

		uint16_t MethodCount() const;

		MethodInfo* GetMethod(size_t index) const;

		MethodInfo* GetMethod(const std::string& methodName) const;

		MethodInfo* GetMethod(const std::string& methodName, size_t paramsCount) const;

		MethodInfo* GetMethod(const std::string& methodName, const std::string& name, size_t sameMethodNameSkip = 0, int indexOffset = 0) const;

		MethodInfo* GetMethodByPattern(const SignaturePattern& pattern) const;

		MethodInfo* GetMethodByPatternNullable(const SignaturePattern& pattern) const;

		bool CompareFieldPattern(const FieldPattern* fieldPattern) const;

		bool HasMethods() const;

		bool IsMethodsInitialized() const;

		bool IsValueType() const;

		bool IsEnum() const;

		uint32_t GetFlags();

		Type* GetType() const;
	};
}