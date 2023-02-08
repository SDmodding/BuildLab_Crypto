#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <Windows.h>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("[ - ] You didnt launch program with args: [key] [file_name]\n");
		return 1;
	}

	std::string m_Key			= argv[1];
	std::string m_InputFileName	= argv[2];
	
	FILE* m_InputFile = fopen(&m_InputFileName[0], "rb");
	if (!m_InputFile)
	{
		printf("[ - ] Couldn't open file: %s\n", &m_InputFileName[0]);
		return 1;
	}

	fseek(m_InputFile, 0L, SEEK_END);	

	size_t m_AllocSize	= static_cast<size_t>(ftell(m_InputFile));
	char* m_AllocData	= reinterpret_cast<char*>(malloc(m_AllocSize));
	if (m_AllocData)
	{
		printf("[ + ] Successfully read input file with size of: %u\n", m_AllocSize);
		fseek(m_InputFile, 0L, SEEK_SET);
		fread(m_AllocData, sizeof(char), m_AllocSize, m_InputFile);
		fclose(m_InputFile);
	}
	else
	{
		printf("[ - ] Couldn't allocate buffer for input file\n");
		fclose(m_InputFile);

		return 1;
	}

	// Encrypt/Decrypt
	for (size_t i = 0; m_AllocSize > i; ++i)
		m_AllocData[i] ^= m_Key[i % m_Key.size()];

	std::string m_OutputFileName = m_InputFileName.substr(0, min(m_InputFileName.size(), m_InputFileName.find_last_of(".")));
	if (m_InputFileName.find(".dat") == std::string::npos) // Is Raw File?
		m_OutputFileName += ".dat";
	else
		m_OutputFileName += ".txt";

	FILE* m_OutputFile = fopen(&m_OutputFileName[0], "wb");
	if (!m_OutputFile)
	{
		printf("[ ERROR ] Couldn't open file: %s\n", &m_OutputFileName[0]);
		return 1;
	}

	printf("[ + ] Successfully write output file with size of: %u\n", m_AllocSize);
	fwrite(m_AllocData, sizeof(char), m_AllocSize, m_OutputFile);
	fclose(m_OutputFile);

	return 0;
}