/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#ifdef DEBUG
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>

#include "securearray.hh"

namespace yapet {
class Logger {
   public:
    Logger(const std::string& filename = "/tmp/yapet_debug.log")
        : _file{filename, _file.out | _file.app} {}

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    void log(const std::string& message) {
        auto t{std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now())};

        _file << "[" << std::put_time(std::localtime(&t), "%FT%T") << "] "
              << message << std::endl;
    }

   private:
    std::fstream _file;
};

inline std::string secureArrayToString(const SecureArray& secureArray) {
    std::string str;
    for (SecureArray::size_type i = 0; i < secureArray.size(); i++) {
        str += static_cast<const char>(secureArray[i]);
    }

    return str;
}

extern Logger logger;
}  // namespace yapet
#define LOG_MESSAGE(x) (yapet::logger.log(x))
#else
#define LOG_MESSAGE(x)
#endif

#endif