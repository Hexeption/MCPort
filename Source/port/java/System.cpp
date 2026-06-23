//
// Created by Keir Davis on 22/06/2026.
//

#include "System.h"

#include <cstdlib>
#include <chrono>
#include <string>

namespace System {
    long_t currentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    long_t nanoTime() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    jstring getenvAsJString(const char *key) {
        if (const char *value = std::getenv(key)) {
            return String::fromUtf8(value);
        }
        return {};
    }

    jstring getProperty(const jstring &key) {
        if (key == u"user.home") {
#ifdef _WIN32
            return getenvAsJString("USERPROFILE");
#else
            return getenvAsJString("HOME");
#endif
        }

        if (key == u"os.name") {
#ifdef _WIN32
            return u"Windows";
#elif defined(__APPLE__)
            return u"Mac OS X";
#elif defined(__linux__)
            return u"Linux";
#elif defined(__unix__)
            return u"Unix";
#else
            return u"Unknown";
#endif
        }

        return {};
    }

    jstring getProperty(const jstring &key, const jstring &defaultValue) {
        jstring value = getProperty(key);
        return value.empty() ? defaultValue : value;
    }
}
