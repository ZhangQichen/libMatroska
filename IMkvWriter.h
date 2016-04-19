#pragma once
#include <fstream>
#include <string>
#ifndef  IMKV_WRITER_H
#define IMKV_WRITER_H

namespace MkvParser
{
	class MkvWriter
	{
	public:
		MkvWriter(std::string filename) : FileName(filename)
		{
		}
		std::string FileName;
		bool Open()
		{
			m_os.open(FileName, std::ios_base::out | std::ios_base::binary);
			return m_os.is_open();
		}

	private:
		std::ofstream m_os;
	};
}

#endif // ! IMKV_WRITER_H
