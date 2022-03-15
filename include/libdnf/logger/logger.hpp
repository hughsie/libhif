/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LIBDNF_LOGGER_LOGGER_HPP
#define LIBDNF_LOGGER_LOGGER_HPP

#include "libdnf/utils/format.hpp"

#include <unistd.h>

#include <array>
#include <chrono>
#include <string>


namespace libdnf {

/// Logger is an abstract interface used for logging.
/// An implementation (inherited class) can call callbacks, log the messages to memory, file, or somewhere else.
class Logger {
public:
    Logger() = default;
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger & operator=(const Logger &) = delete;
    Logger & operator=(Logger &&) = delete;
    virtual ~Logger() = default;

    enum class Level : unsigned int { CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG, TRACE };

    static constexpr const char * level_to_cstr(Level level) noexcept {
        auto ilevel = static_cast<unsigned int>(level);
        return ilevel >= LEVEL_C_STR.size() ? "UNDEFINED" : LEVEL_C_STR[ilevel];
    }

    template <typename... Ss>
    void critical(std::string_view format, Ss &&... args) {
        log(Level::CRITICAL, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void error(std::string_view format, Ss &&... args) {
        log(Level::ERROR, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void warning(std::string_view format, Ss &&... args) {
        log(Level::WARNING, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void notice(std::string_view format, Ss &&... args) {
        log(Level::NOTICE, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void info(std::string_view format, Ss &&... args) {
        log(Level::INFO, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void debug(std::string_view format, Ss &&... args) {
        log(Level::DEBUG, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void trace(std::string_view format, Ss &&... args) {
        log(Level::TRACE, format, std::forward<Ss>(args)...);
    }

    template <typename... Ss>
    void log(Level level, std::string_view format, Ss &&... args) {
        log_line(level, libdnf::utils::sformat(format, std::forward<Ss>(args)...));
    }

    virtual void log_line(Level level, const std::string & message) noexcept;

    virtual void write(
        const std::chrono::time_point<std::chrono::system_clock> & time,
        pid_t pid,
        Level level,
        const std::string & message) noexcept = 0;

private:
#ifndef SWIG
    static constexpr std::array<const char *, static_cast<unsigned int>(Level::TRACE) + 1> LEVEL_C_STR = {
        "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO", "DEBUG", "TRACE"};
#endif
};

class StringLogger : public Logger {
public:
    void write(
        const std::chrono::time_point<std::chrono::system_clock> & time,
        pid_t pid,
        Level level,
        const std::string & message) noexcept override;

    virtual void write(const char * line) noexcept = 0;
};

}  // namespace libdnf

#endif
