#pragma once

#include <signal.h>

namespace S4{
class signalhandler_t{

public:
  static bool getSighup() { return _got_sighup; }
  static bool getSigint() { return _got_sigint; }

  static void HookupHandler() ;

  static void UnhookHandler();

  static void dummyInt() { _got_sigint = true; _got_sighup = true; }

private:
  static bool _got_sighup;
  static bool _got_sigint;
  static bool _already_hooked_up;

  static void handle_signal(int signal) {
      switch (signal) {
#ifdef _WIN32
      case SIGTERM:
      case SIGABRT:
      case SIGBREAK:
#else
      case SIGHUP:
#endif
        _got_sighup = true;
        break;
      case SIGINT:
        _got_sigint = true;
        break;
      }
  }

};
}