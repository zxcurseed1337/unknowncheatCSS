#include "pch.h"
#include "Modules.h"

const Utility::Module& Utility::ModuleStorage::operator[](const std::string_view name) {
	if (!storage.contains(name.data()))
		storage.insert({ name.data(), Module(name) });
		
	return storage[name.data()];
}

Utility::Module::Module(std::string_view name) : name(name), hmod(GetModuleHandleA(name.data())), base(reinterpret_cast<uintptr_t>(hmod)), size(0), offsets(*this) {
	if (hmod == 0)
		throw std::runtime_error(std::format("No such module! {}", name));

	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), hmod, &modinfo, sizeof(MODULEINFO));

	const_cast<uintptr_t&>(size) = modinfo.SizeOfImage;

	offsets.base = base;
}

void Utility::Module::OffsetStorage::make(std::string_view name, ptrdiff_t offset) {
	storage.insert({ name.data(), parent.base + offset});
}

void Utility::Module::OffsetStorage::make(std::string_view name, std::function<ptrdiff_t(uintptr_t start, uintptr_t end)> search_fn) {
	make(name, search_fn(parent.base, parent.size));
}

const uintptr_t Utility::Module::OffsetStorage::operator[](std::string_view name) const {
	return storage.at(name.data());
}

void Utility::Module::OffsetStorage::dump() {
	for (const auto& offset : storage) {
		std::println("{} in {} @ offset {:16}, absoulte addr {:16}", offset.first, parent.name, offset.second - parent.base, offset.second);
	}
}
