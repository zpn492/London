/*
* @file Socket.h
* @brief Abstract class to perform API global operations
*
* @author Mohamed Amine Mzoughi <mohamed-amine.mzoughi@laposte.net>
* @date 2017-02-10
*/

#ifndef INCLUDE_ASOCKET_H_
#define INCLUDE_ASOCKET_H_

#include <cstdio>         // snprintf
#include <exception>
#include <functional>
#include <memory>
#include <stdarg.h>       // va_start, etc.
#include <stdexcept>

#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment(lib,"WS2_32.lib")


#include <limits>
#define ACCEPT_WAIT_INF_DELAY std::numeric_limits<size_t>::max()

class ASocket
{
public:
   // Public definitions
   //typedef std::function<int(void*, double, double, double, double)> ProgressFnCallback;
   typedef std::function<void(const std::string&)>                   LogFnCallback;

   // socket file descriptor id
   typedef SOCKET Socket;

   enum SettingsFlag
   {
      NO_FLAGS = 0x00,
      ENABLE_LOG = 0x01,
      ALL_FLAGS = 0xFF
   };

   /* Please provide your logger thread-safe routine, otherwise, you can turn off
   * error log messages printing by not using the flag ALL_FLAGS or ENABLE_LOG */
   explicit ASocket(const LogFnCallback& oLogger,
                    const SettingsFlag eSettings = ALL_FLAGS);
   virtual ~ASocket() = 0;

   static int SelectSockets(const Socket* pSocketsToSelect, const size_t count,
                            const size_t msec, size_t& selectedIndex);

   int SelectSocket(const Socket sd, const size_t msec);

   static struct timeval TimevalFromMsec(unsigned int time_msec);

   // String Helpers
   static std::string StringFormat(const std::string strFormat, ...);

protected:
   // Log printer callback
   /*mutable*/const LogFnCallback         m_oLog;

   SettingsFlag         m_eSettingsFlags;

   static WSADATA s_wsaData;

private:
   friend class SocketGlobalInitializer;
   class SocketGlobalInitializer {
   public:
      static SocketGlobalInitializer& instance();

      SocketGlobalInitializer(SocketGlobalInitializer const&) = delete;
      SocketGlobalInitializer(SocketGlobalInitializer&&) = delete;

      SocketGlobalInitializer& operator=(SocketGlobalInitializer const&) = delete;
      SocketGlobalInitializer& operator=(SocketGlobalInitializer&&) = delete;

      ~SocketGlobalInitializer();

   private:
      SocketGlobalInitializer();
   };
   SocketGlobalInitializer& m_globalInitializer;
};

class EResolveError : public std::logic_error
{
public:
   explicit EResolveError(const std::string &strMsg) : std::logic_error(strMsg) {}
};

#endif
