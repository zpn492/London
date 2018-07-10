#include "../../SimpleJust.h"

#define FGREEN   10
#define FRED     12
#define FYELLOW  14
#define BGREEN   0x0020
#define BRED     0x0040

#ifndef LOGGER_H_
#define LOGGER_H_

class Logger
    {
    public:
    Logger()
        {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        /* Save current attributes */
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;
		doDebugPrint = true;
        };
    void setColor(int);
    void resetColor();
	
	/* stdout on the same line fx timer, processed etc.
	 * end the line with function end()
	 */
	void write_inline(std::string);
    void write(int frontcolor, const char message[]);
	void end();
	void setDebug(bool debug) { doDebugPrint = debug; };
	bool debug() { return doDebugPrint; };
    private:
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
	bool doDebugPrint;
    };
#endif