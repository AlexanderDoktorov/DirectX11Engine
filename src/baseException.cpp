#include "baseException.h"

baseException::baseException(int codeLine, std::string_view codeFileName) noexcept
	: 
	codeLine(codeLine),
	codeFileName(codeFileName)
{

}

int baseException::GetLine() const noexcept
{
	return codeLine;
}

std::string baseException::GetFileName() const noexcept
{
	return codeFileName;
}

std::string baseException::what() const
{
	using namespace std::string_literals;
	return "[File] "s + codeFileName + "\n[Line] "s + std::to_string(codeLine) + '\n';
}

