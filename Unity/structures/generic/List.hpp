#pragma once
#include <memory>
#include <cstring>
#include "Array.hpp"

namespace IL2CPP
{
	template <typename T>
	struct List : private Object
	{
		private:
		Array<T>* items;
		int size;
		int version;
		IMPORT::Il2CppObject* syncRoot;

		public:
		T Get(int index)
		{
			return items->vector[index];
		}

		void Set(T value, int index)
		{
			items->vector[index] = value;
		}

		void ForEach(std::function<void(T)> func)
		{
			for (int i = 0; i < size; ++i)
			{
				func(items->vector[i]);
			}
		}

		void ForEach(std::function<void(T, int)> func)
		{
			for (int i = 0; i < size; ++i)
			{
				func(items->m_Ivectortems[i], i);
			}
		}
	};

}

