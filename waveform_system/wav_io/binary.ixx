module;

#include <sstream>
#include <fstream>
#include <string>

export module sense.binary;

export namespace sense
{
	class Binary
	{
		std::stringstream data;

	public:
		Binary()noexcept
		{

		}
		Binary(const std::string &fileName)
		{
			read(fileName);
		}
		void read(const std::string &fileName)
		{
			std::ifstream file(fileName, std::ios::binary);
			if(!file)
			{
				throw "Binary::read : File Open Error";
			}
			data << file.rdbuf();
		}
		void read(const void *buffer, size_t size)
		{
			data.write(static_cast<const char *>(buffer), size);
		}
		void read(const Binary &data)
		{
			this->data << data.data.rdbuf();
		}
		void write(const std::string &fileName)const
		{
			std::ofstream file(fileName, std::ios::binary);
			if(!file)
			{
				throw "Binary::write : File Open Error";
			}
			file << data.rdbuf();
		}
		void write(void *buffer, size_t size)
		{
			data.read(static_cast<char *>(buffer), size);
		}
		void write(Binary &data)const
		{
			data.read(*this);
		}
		void seek(long long offset)
		{
			data.seekg(offset, std::ios_base::cur);
		}
		size_t getSeekPosition()
		{
			return data.tellg();
		}
		size_t getSize()
		{
			const size_t currentPosition = data.tellg();
			data.seekg(0, std::ios::end);
			const size_t size = data.tellg();
			data.seekg(currentPosition);
			return size;
		}
		bool isEOF()
		{
			return getSeekPosition() == getSize();
		}
	};
}