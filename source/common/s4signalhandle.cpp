
#include "common/s4signalhandle.h"
#include "common/s4logger.h"

namespace S4{
    
bool signalhandler_t::_got_sighup = false;
bool signalhandler_t::_got_sigint = false;
bool signalhandler_t::_already_hooked_up = false;

void signalhandler_t::HookupHandler(){
    if (_already_hooked_up) {
      INFO("Tried to hookup signal handlers more than once.");
    }
    _already_hooked_up = true;
#ifdef _WIN32
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGABRT, handle_signal);
#else
    struct sigaction sa;
    // Setup the handler
    sa.sa_handler = &handle_signal;
    // Restart the system call, if at all possible
    sa.sa_flags = SA_RESTART;
    // Block every signal during the handler
    sigfillset(&sa.sa_mask);
    // Intercept SIGHUP and SIGINT
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
      ERR("Cannot install SIGHUP handler.");
    }
    if (sigaction(SIGINT, &sa, NULL) == -1) {
      ERR("Cannot install SIGINT handler.");
    }
#endif
}

void signalhandler_t::UnhookHandler() {
    if (_already_hooked_up) {
#ifdef _WIN32
        signal(SIGINT, SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGABRT, SIG_DFL);
#else
      struct sigaction sa;
      // Setup the sighub handler
      sa.sa_handler = SIG_DFL;
      // Restart the system call, if at all possible
      sa.sa_flags = SA_RESTART;
      // Block every signal during the handler
      sigfillset(&sa.sa_mask);
      // Intercept SIGHUP and SIGINT
      if (sigaction(SIGHUP, &sa, NULL) == -1) {
        ERR("Cannot uninstall SIGHUP handler.");
      }
      if (sigaction(SIGINT, &sa, NULL) == -1) {
        ERR("Cannot uninstall SIGINT handler.");
      }
#endif

      _already_hooked_up = false;
    }
}

}