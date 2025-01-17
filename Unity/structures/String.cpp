#include "String.hpp"
#include <memory>
#include <cstring>

namespace IL2CPP
{
	String* String::Create(const char* str)
	{
		return (String*)IMPORT::il2cpp_string_new(str);
	}

	String* String::Create(const std::string& str)
	{
		return (String*)IMPORT::il2cpp_string_new(str.c_str());
	}

	bool String::Equals(String* str)
	{
		static auto func = (bool* (*)(String*, String*)) DefaultTypeClass::String->GetMethod(0x15)->GetPointer();
		return func(this, str);
	}

	bool String::Equals(const char* str)
	{
		return this->Equals(Create(str));
	}

	bool String::Equals(const std::string& str)
	{
		return this->Equals(Create(str.c_str()));
	}

	bool String::Contains(String* str)
	{
		static auto func = (bool* (*)(String*, String*)) DefaultTypeClass::String->GetMethod(0x65)->GetPointer();
		return func(this, str);
	}

	bool String::Contains(const char* str)
	{
		return this->Contains(Create(str));
	}

	bool String::Contains(const std::string& str)
	{
		return this->Contains(Create(str.c_str()));
	}

	bool String::IsNullOrEmpty()
	{
		static auto func = (bool* (*)(String*)) DefaultTypeClass::String->GetMethod(0x9e)->GetPointer();

		return func(this);
	}

	int String::GetLength() const
	{
		return this->length;
	}

	std::string String::ToString()
	{
		static Class* encoding = DefaultImage::Corlib->GetClass("System.Text.Encoding");

		static auto GetUTF8 = (void* (*)()) encoding->GetMethod("get_UTF8", 0)->GetPointer();

		static auto GetBytes = (IMPORT::Il2CppArraySize* (*)(void*, String*)) encoding->GetMethod("GetBytes", 1)->GetPointer();

		IMPORT::Il2CppArraySize* arr = GetBytes(GetUTF8(), this);

		std::unique_ptr<char[]> dest(new char[arr->max_length + 1]);
		std::memcpy(dest.get(), arr->vector, arr->max_length);

		dest[arr->max_length] = '\0';

		return { dest.release() };
	}

	//static String* String::FromArray(Array<char>* obj)
	//{
	//	static Class* encoding = (Class*)IMPORT::il2cpp_class_from_name(
	//		(IMPORT::Il2CppImage*)CommonImages::Corlib,
	//		"System.Text",
	//		"Encoding"
	//	);
	//
	//	static auto GetUTF8 = (void* (*)()) encoding->GetMethod("get_UTF8", 0)->GetPointer();
	//
	//	static auto GetString = (String * (*)(void*, void*)) encoding->GetMethod("GetString", 1)->GetPointer();
	//
	//	return GetString(GetUTF8(), obj);
	//}

	//Array<char>* String::ToArray()
	//{
	//	static Class* encoding = (Class*)IMPORT::il2cpp_class_from_name(
	//		(IMPORT::Il2CppImage*)CommonImages::Corlib,
	//		"System.Text",
	//		"Encoding"
	//	);
	//	static auto GetUTF8 = (void* (*)()) encoding->GetMethod("get_UTF8", 0)->GetPointer();
	//	static auto GetBytes = (Array<char>*(*)(void*, void*)) encoding->GetMethod("GetBytes", 1)->GetPointer();
	//	return GetBytes(GetUTF8(), this);
	//}
}