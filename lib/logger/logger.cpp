#include "Logger.h"

/* *************************************************** */
/* Public ******************************************** */
/* *************************************************** */
void Logger::setColor(int frontcolor)
    {
    SetConsoleTextAttribute(hConsole, frontcolor | 0x0008);
    };
void Logger::resetColor()
    {
    SetConsoleTextAttribute(hConsole, saved_attributes);
    };
void Logger::write_inline(std::string s)
	{
	setColor(FYELLOW);
	std::cout << "\r" << s.c_str();
	resetColor();
	};
void Logger::write(int frontcolor, const char message[])
	{
	if(frontcolor == 0)
		{
		std::cout << message << std::endl;
		}
	else
		{
		setColor(frontcolor);
		std::cout << message << std::endl;
		resetColor();
		}
	};
void Logger::end()
	{
	std::cout << "" << std::endl;
	};