#pragma once

namespace Utility {
	class Module {
	public:
		class OffsetStorage {
			std::unordered_map<const char*, uintptr_t> storage;
			uintptr_t base;
			const Module& parent;
		public:
			OffsetStorage(const Module& parent) : base(0), parent(parent) {}
			OffsetStorage(const Module& parent, uintptr_t base) : base(base), parent(parent) {}

			void make(std::string_view name, ptrdiff_t offset);
			void make(std::string_view name, std::function<ptrdiff_t(uintptr_t start, uintptr_t end)> search_fn);
			const uintptr_t operator[](std::string_view) const;

			void dump();

			template<typename T>
			const T get(std::string_view name) const {
				return reinterpret_cast<T>((*this)[name]);
			}
			friend Module;
		} offsets;

		const HMODULE hmod;
		const uintptr_t base;

		const size_t size;
		const std::string name;

		Module() : hmod(NULL), base(0), size(0), name(""), offsets(*this) {};
		Module(std::string_view name);

		template<typename T>
		const T get_export(const std::string_view name) {
			return reinterpret_cast<T>(GetProcAddress(hmod, name.data()));
		}

		template<typename T>
		const T get_interface(const std::string_view iface_name) const {
			typedef void* (*CreateInterfaceFn)(const char* name, int* return_code);
			const FARPROC pfCreateAddr = GetProcAddress(hmod, "CreateInterface");

			if (!pfCreateAddr)
				throw std::runtime_error( std::format("Module {} doesn't have a 'CreateInterface' export.", name));


			const CreateInterfaceFn pfCreateInterface = reinterpret_cast<CreateInterfaceFn>(pfCreateAddr);
			const auto ret = pfCreateInterface(iface_name.data(), nullptr);

			if (ret == nullptr)
				throw std::runtime_error(std::format("Module {} does not contain interface {}.", name, iface_name));

			return static_cast<T>(ret);
		}
	};

	class ModuleStorage {
		std::unordered_map<const char*, Module> storage;
	public: 
		const Module& operator[](const std::string_view name);
	};

	inline ModuleStorage modules;
}

#define enginedll Utility::modules["engine.dll"] // macros for ease of access

