#pragma once
namespace Utility {

    class MemAddr final
    {
    private:
        uintptr_t m_addr{};

    public:
        MemAddr() = default;
        MemAddr(const uintptr_t addr);

    public:
        MemAddr& operator=(const MemAddr& other);
        MemAddr operator+(const MemAddr& other) const;
        MemAddr operator-(const MemAddr& other) const;
        std::strong_ordering operator<=>(const MemAddr& other) const;
        bool operator==(const MemAddr& other) const;
        explicit operator bool() const;

    public:
        uintptr_t get() const;
        void set(const uintptr_t addr);

    public:
        MemAddr offset(const size_t offset);
        MemAddr deref(size_t count = 1);
        /// Used to fix things such as function calls, JMP's and rip addressing
        MemAddr fixRip(const size_t offset = 1);

    public:
        template <typename T>
        T cast()
        {
            return reinterpret_cast<T>(m_addr);
        }

        template <typename T, typename... Args>
        inline T call(Args... args) const
        {
            return reinterpret_cast<T(__fastcall*)(Args...)>(m_addr)(args...);
        }
    };

    MemAddr find_bytes(const Module& mod, std::string_view bytes);
	MemAddr find_virtual(void* const object, const size_t vidx);

	template <size_t idx, typename T, typename... Args>
	inline T callVirtual(void* const base, Args... args)
	{
		return reinterpret_cast<T(__fastcall*)(void*, Args...)>(find_virtual(base, idx).get())(base, args...);
	}
}

