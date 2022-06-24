#pragma once

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

template<typename T>
concept VarType = std::is_same_v<int, T> || std::is_same_v<int64_t, T>;

class PacketStream
{
public:
	PacketStream()
		: m_Cursor(0ULL)
	{
	}

	PacketStream(uint8_t* data, size_t size)
		: m_Data(data, data + size), m_Cursor(0ULL)
	{
	}

	PacketStream(const std::vector<uint8_t>& data)
		: m_Data(data), m_Cursor(0ULL)
	{
	}

	inline const std::vector<uint8_t>& GetData() { return m_Data; }
	inline uint64_t GetCursor() { return m_Cursor; }

	template<typename T>
	T Read(uint64_t offset = 0)
	{
		m_Cursor += offset;
		if (m_Cursor >= m_Data.size())
			return 0;

		uint8_t data[sizeof(T)];
		for (int i = 0; i < sizeof(T); i++)
			data[i] = m_Data[m_Cursor + sizeof(T) - 1 - i];

		m_Cursor += sizeof(T);

		return *(T*)data;
	}

	std::vector<uint8_t> ReadArray(uint64_t offset = 0)
	{
		m_Cursor += offset;

		int length = ReadVar<int>();
		auto value = std::vector<uint8_t>(&m_Data[m_Cursor], &m_Data[m_Cursor] + length);
		m_Cursor += length;

		return value;
	}

	std::string ReadString(uint64_t offset = 0)
	{
		m_Cursor += offset;

		int length = ReadVar<int>();
		auto value = std::string(&m_Data[m_Cursor], &m_Data[m_Cursor] + length);
		m_Cursor += length;

		return value;
	}

	template<VarType T>
	T ReadVar(uint64_t offset = 0)
	{
		m_Cursor += offset;

		T value = 0;
		int position = 0;

		while (true)
		{
			auto currentByte = m_Data[m_Cursor++];
			value |= (T)(currentByte & SEGMENT_BITS) << position;

			if ((currentByte & CONTINUE_BIT) == 0)
				break;

			position += 7;

			if (position >= sizeof(T) * 8)
				return 0;
		}

		return value;
	}

	template<typename T>
	void Write(const T& value, uint64_t offset = 0)
	{
		m_Cursor += offset;
		
		EnsureWriteableSize(sizeof(T));

		for (int i = 0; i < sizeof(T); i++)
			m_Data[m_Cursor + i] = *(uint8_t*)(&value + sizeof(T) - 1 - i);

		m_Cursor += sizeof(T);
	}

	void WriteBytes(const std::vector<uint8_t>& data, uint64_t offset = 0)
	{
		m_Cursor += offset;

		EnsureWriteableSize(data.size());
		std::memcpy(&m_Data[m_Cursor], &data[0], data.size());
		m_Cursor += data.size();
	}

	template<VarType T>
	std::vector<uint8_t> GetEncodedVar(T value)
	{
		std::vector<uint8_t> bytes;
		while (true)
		{
			if ((value & ~SEGMENT_BITS) == 0)
			{
				bytes.push_back(value);
				break;
			}

			bytes.push_back((value & SEGMENT_BITS) | CONTINUE_BIT);
			value >>= 7;
		}

		return bytes;
	}

	template<VarType T>
	void WriteVar(T value, uint64_t offset = 0)
	{
		m_Cursor += offset;

		auto encoded = GetEncodedVar(value);
		EnsureWriteableSize(encoded.size());

		WriteBytes(encoded);
	}

	void WriteArray(const std::vector<uint8_t>& value, uint64_t offset = 0)
	{
		m_Cursor += offset;

		auto varLength = GetEncodedVar<int>((int)value.size());
		EnsureWriteableSize(varLength.size() + value.size());

		WriteBytes(varLength);
		WriteBytes(value);
	}

	void WriteString(std::string_view value, uint64_t offset = 0)
	{
		m_Cursor += offset;

		auto varLength = GetEncodedVar<int>((int)value.size());
		EnsureWriteableSize(varLength.size() + value.size());

		WriteBytes(varLength);

		if (value.size() > 0)
		{
			std::memcpy(&m_Data[m_Cursor], value.data(), value.size());
			m_Cursor += value.size();
		}
	}

private:
	void EnsureWriteableSize(size_t size)
	{
		if (m_Cursor + size > m_Data.size())
		{
			m_Data.resize(m_Cursor + size);
		}
	}

private:
	std::vector<uint8_t> m_Data;
	uint64_t m_Cursor;
};
