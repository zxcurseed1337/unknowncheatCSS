#include "pch.h"
#include "memory.h"


using namespace Utility;

MemAddr Utility::find_bytes(const Module& mod, std::string_view bytes)
{
	auto sigToBytes = [](const char* const sig) -> std::vector<int>
		{
			std::vector<int> bytes{};

			char* const start{ const_cast<char*>(sig) };
			char* const end{ const_cast<char*>(sig) + strlen(sig) };

			for (char* current{ start }; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?') {
						++current;
					}

					bytes.push_back(-1);
				}

				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}

			return bytes;
		};

	const std::vector<int> pattern_bytes{ sigToBytes(bytes.data()) };
	const byte* const image_bytes{ reinterpret_cast<byte*>(mod.base) };
	const size_t signature_size{ pattern_bytes.size() };
	const int* signature_bytes{ pattern_bytes.data() };

	for (unsigned long i{}; i < mod.size - signature_size; ++i)
	{
		bool byte_sequence_found{ true };

		for (unsigned long j{}; j < signature_size; ++j)
		{
			if (image_bytes[i + j] != signature_bytes[j] && signature_bytes[j] != -1) {
				byte_sequence_found = false;
				break;
			}
		}

		if (byte_sequence_found) {
			return reinterpret_cast<uintptr_t>(&image_bytes[i]);
		}
	}

	return 0;
}

MemAddr Utility::find_virtual(void* const object, const size_t vidx)
{
	return reinterpret_cast<uintptr_t>((*static_cast<void***>(object))[vidx]);
}

MemAddr::MemAddr(const uintptr_t addr)
{
	m_addr = addr;
}

MemAddr& MemAddr::operator=(const MemAddr& other)
{
	if (this != &other) {
		m_addr = other.m_addr;
	}

	return *this;
}

MemAddr MemAddr::operator+(const MemAddr& other) const
{
	return { m_addr + other.m_addr };
}

MemAddr MemAddr::operator-(const MemAddr& other) const
{
	return { m_addr - other.m_addr };
}

std::strong_ordering MemAddr::operator<=>(const MemAddr& other) const
{
	return m_addr <=> other.m_addr;
}

bool MemAddr::operator==(const MemAddr& other) const
{
	return m_addr == other.m_addr;
}

MemAddr::operator bool() const
{
	return m_addr > 0;
}

uintptr_t MemAddr::get() const
{
	return m_addr;
}

void MemAddr::set(const uintptr_t addr)
{
	m_addr = addr;
}

MemAddr MemAddr::offset(const size_t offset)
{
	if (m_addr) {
		m_addr += offset;
	}

	return *this;
}

MemAddr MemAddr::deref(size_t count)
{
	uintptr_t out = m_addr;
	while (out && count > 0)
	{
		out = *reinterpret_cast<uintptr_t*>(out);

		if (!out)
			return m_addr;

		count--;
	}

	m_addr = out;

	return *this;
}

MemAddr MemAddr::fixRip(const size_t offset)
{
	if (m_addr && offset)
	{
		uintptr_t base = m_addr + offset;

		auto rip_address = *reinterpret_cast<int32_t*>(base);

		base += 4 + rip_address;

		m_addr = base;
	}

	return *this;
}