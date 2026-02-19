#include "pch.h"
#include "Hooks.h"

namespace Utility {
	class DummyHookGuard {
	public:
		std::string name;
	protected:
		DummyHookGuard(std::string_view name) {}
		virtual ~DummyHookGuard() = 0;
	};

	class MinHookHookGuard : public DummyHookGuard {
	private:
		MinHookHookGuard(std::string_view name, uintptr_t what, uintptr_t with, uintptr_t& original);
		~MinHookHookGuard() {};

		friend class HookGuardStorage;
	};

	class VTableHookGuard : public DummyHookGuard {
	private:
		VTableHookGuard();

		friend class HookGuardStorage;
	};

	class WndProcHookGuard : public DummyHookGuard {
	private:
		WndProcHookGuard(); //: DummyHook("WndProc") {}

		friend class HookGuardStorage;
	};

	class HookGuardStorage {

	};
}