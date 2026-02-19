// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу

#include "pch.h"

void hook(uintptr_t what, uintptr_t with, uintptr_t& original) {
	if (MH_CreateHook(reinterpret_cast<LPVOID>(what), reinterpret_cast<LPVOID>(with), reinterpret_cast<LPVOID*>(&original)) != MH_OK)
	{
		std::println("Failed to hook!");
		return;
	}
}

// При использовании предварительно скомпилированных заголовочных файлов необходим следующий файл исходного кода для выполнения сборки.
