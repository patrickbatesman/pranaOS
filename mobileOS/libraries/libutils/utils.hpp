/**
 * @file utils.hpp
 * @author Krisna Pranav
 * @brief UTILS
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */
#pragma once


#include "split.hpp"
#include <log/log.hpp>
#include <magic_enum.hpp>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <tuple>
#include <type_traits>
#include <functional>

namespace Utils {
    /**
     * @brief WHITESPACE, secondsInMinute
     * 
     */
    inline constexpr auto WHITESPACE       = " \n\r\t\f\v";
    constexpr unsigned int secondsInMinute = 60;

    /**
     * @brief isByte
     * 
     * @tparam T 
     */
    template <typename T>
    inline constexpr bool is_byte_v = std::is_integral_v<T> && sizeof(T) == sizeof(std::uint8_t);

    template <typename T>
    using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

    /**
     * @brief halfByteToHex
     * 
     * @tparam T 
     * @param c 
     * @return char 
     */
    template <typename T, std::enable_if_t<is_byte_v<T>, int> = 0>
    inline char halfByteToHex(T c) {
        return c < 10 ? '0' + c : 'a' + (c - 10);
    }

    /**
     * @brief byteToHex
     * 
     * @tparam T 
     * @param c 
     * @return std::string 
     */
    template <typename T, std::enable_if_t<is_byte_v<T>, int> = 0>
    inline std::string byteToHex(T c) {
        return {halfByteToHex<std::uint8_t>((c & 0xF0) >> 4), halfByteToHex<std::uint8_t>(c & 0x0F)};
    }

    /**
     * @brief halfHexToByte
     * 
     * @tparam T 
     * @param c 
     * @return T 
     */
    template <typename T = std::uint8_t, std::enable_if_t<is_byte_v<T>, int> = 0>
    inline T halfHexToByte(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        throw std::invalid_argument("Unexpected hex digit");
        return 0;
    }

    /**
     * @brief hexToBytes
     * 
     * @tparam Bytes 
     * @param hex 
     * @return Bytes 
     */
    template <typename Bytes = std::vector<std::uint8_t>,
              std::enable_if_t<is_byte_v<typename Bytes::value_type>, int> = 0>
    inline Bytes hexToBytes(const std::string &hex) {
        using Value                      = typename Bytes::value_type;
        const std::size_t bytesCount     = (hex.size() + 1) / 2;
        const std::size_t fullBytesCount = hex.size() / 2;
        Bytes bytes(bytesCount, Value(0));
        for (std::size_t i = 0; i < fullBytesCount; ++i) {
            const std::size_t offset = i * 2;
            bytes[i]                 = (halfHexToByte<Value>(hex[offset]) << 4) | halfHexToByte<Value>(hex[offset + 1]);
        }
        if (fullBytesCount < bytesCount) {
            bytes.back() = halfHexToByte<Value>(hex.back());
        }
        return bytes;
    }
    
    /**
     * @brief startsWith
     * 
     * @param str 
     * @param prefix 
     * @return true 
     * @return false 
     */
    inline bool startsWith(std::string const &str, std::string const &prefix) {
        return prefix.size() <= str.size() && std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    /**
     * @brief endsWith
     * 
     * @param str 
     * @param suffix 
     * @return true 
     * @return false 
     */
    inline bool endsWith(std::string const &str, std::string const &suffix) {
        return suffix.size() <= str.size() && std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }

    /**
     * @brief removeNewLines
     * 
     * @param s 
     * @return std::string 
     */
    static inline std::string removeNewLines(const std::string &s) {
        std::string retStr = s;
        retStr.erase(std::remove(retStr.begin(), retStr.end(), '\r'), retStr.end());
        retStr.erase(std::remove(retStr.begin(), retStr.end(), '\n'), retStr.end());

        return retStr;
    }

    /**
     * @brief isNumber
     * 
     * @param s 
     * @return true 
     * @return false 
     */
    static inline bool isNumber(const std::string &s) {
        return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

    /**
     * @brief isPhoneNumber
     * 
     * @param s 
     * @return true 
     * @return false 
     */
    static inline bool isPhoneNumber(const std::string &s) {
        return s.find_first_not_of(" +-.#()0123456789") == std::string::npos;
    }

    /**
     * @brief ltrim
     * 
     * @param s 
     * @return std::string 
     */
    static inline std::string ltrim(const std::string &s) {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    /**
     * @brief rtrim
     * 
     * @param s 
     * @return std::string 
     */
    static inline std::string rtrim(const std::string &s) {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    /**
     * @brief trim
     * 
     * @param s 
     * @return std::string 
     */
    static inline std::string trim(const std::string &s) {
        return rtrim(ltrim(s));
    }

    /**
     * @brief addLeadingZeros
     * 
     * @param base 
     * @param minStringLength 
     * @return std::string 
     */
    static inline std::string addLeadingZeros(std::string base, size_t minStringLength = 0) {
        if (base.length() >= minStringLength) {
            return base;
        }
        constexpr auto leadingDigit = '0';
        base.insert(0, minStringLength - base.length(), leadingDigit);
        return base;
    }

    /**
     * @brief toString
     * 
     * @tparam T 
     * @param t 
     * @return std::string 
     */
    template <typename T>
    [[nodiscard]] inline std::string toString(T t) {
        return std::toString(t);
    }

    /**
     * @brief toString
     * 
     * @tparam  
     * @param t 
     * @return std::string 
     */
    template <>
    [[nodiscard]] inline std::string toString<long double>(long double t) {
        uint32_t precision = 6;
        int base           = static_cast<int>(t);
        long double frac   = (t - base) * pow(10, precision);
        auto baseAsStr     = std::to_string(base);
        if (t < 0) {
            frac *= -1;
            if (base == 0) {
                baseAsStr = "-0";
            }
        }

        auto fractionalPart       = static_cast<unsigned long int>(roundl(frac));
        auto fractionalPartLength = std::to_string(fractionalPart).length();
        if (fractionalPartLength > precision) {
            base += 1;
            baseAsStr      = std::to_string(base);
            fractionalPart = 0;
        }
        if (fractionalPart == 0) {
            if (baseAsStr == "-0") {
                return "0";
            }
            return baseAsStr;
        }
        auto fractionalAsStr = std::to_string(fractionalPart);
        if (fractionalAsStr.size() < precision) {
            fractionalAsStr.insert(0, precision - fractionalAsStr.size(), '0');
        }
        if (fractionalAsStr.find_last_not_of('0') != std::string::npos) {
            fractionalAsStr.erase(fractionalAsStr.find_last_not_of('0') + 1);
        }
        return baseAsStr + "." + fractionalAsStr;
    }

    /**
     * @brief toString
     * 
     * @tparam  
     * @param t 
     * @return std::string 
     */
    template <>
    [[nodiscard]] inline std::string toString<float>(float t) {
        return toString(static_cast<long double>(t));
    }

    /**
     * @brief toString
     * 
     * @tparam  
     * @param t 
     * @return std::string 
     */
    template <>
    [[nodiscard]] inline std::string toString<double>(double t) {
        return toString(static_cast<long double>(t));
    }

    /**
     * @brief toString
     * 
     * @tparam  
     * @param value 
     * @return std::string 
     */
    template <>
    [[nodiscard]] inline std::string toString<std::int64_t>(std::int64_t value) {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }

    /**
     * @brief 
     * 
     * @tparam  
     * @param value 
     * @return std::string 
     */
    template <>
    [[nodiscard]] inline std::string toString<std::uint64_t>(std::uint64_t value) {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }

    /**
     * @brief enumToString
     * 
     * @tparam T 
     * @param t 
     * @return const std::string 
     */
    template <typename T>
    [[nodiscard]] const std::string enumToString(const T &t) {
        static_assert(std::is_enum_v<T>);
        return std::string(magic_enum::enum_name(t));
    }

    /**
     * @brief Get the Numeric Value object
     * 
     * @tparam T 
     * @param value 
     * @return T 
     */
    template <typename T>
    [[nodiscard]] T getNumericValue(const std::string &value) {
        static_assert(std::is_arithmetic_v<T>);
        if (value.empty()) {
            return {};
        }

        T ret;
        if constexpr (std::is_same<uint8_t, T>::value || std::is_same<int8_t, T>::value) {
            try {
                ret = std::stoi(value);
            }
            catch (const std::exception &e) {
                LOG_INFO("Invalid input value: %s", e.what());
                ret = 0;
            }

            return ret;
        }

        std::istringstream(value) >> ret;
        return ret;
    }

    /**
     * @brief toNumeric
     * 
     * @param text 
     * @param value 
     * @return true 
     * @return false 
     */
    static inline bool toNumeric(const std::string &text, int &value) {
        try {
            value = std::stoi(text);
        }
        catch (const std::exception &e) {
            LOG_ERROR("toNumeric exception %s", e.what());
            return false;
        }
        return true;
    }

    /**
     * @brief toNumeric
     * 
     * @param text 
     * @return int 
     */
    static inline int toNumeric(const std::string &text) {
        try {
            return std::stoi(text);
        }
        catch (const std::exception &e) {
            LOG_ERROR("toNumeric exception %s", e.what());
            return 0;
        }
    }

    /**
     * @brief computeIncrease
     * 
     * @tparam T 
     * @param currentCount 
     * @param lastCount 
     * @return T 
     */
    template <class T>
    [[nodiscard]] inline T computeIncrease(const T currentCount, const T lastCount) {
        if (currentCount >= lastCount) {
            return currentCount - lastCount;
        }
        else {
            return std::numeric_limits<T>::max() - lastCount + currentCount;
        }
    }

    /**
     * @brief findAndReplaceAll
     * 
     * @param data 
     * @param values 
     * @param getReplaceString 
     */
    static inline void findAndReplaceAll(std::string &data, const std::vector<std::pair<std::string, std::optional<std::string>>> &values, std::function<std::string(int)> getReplaceString = nullptr) {
        int valIdx = 0;
        for (auto &el : values) {
            std::string preStr, replaceStr, postStr;
            const std::string &toSearch = el.first;

            if (toSearch.empty()) {
                continue;
            }

            auto begin_pos = data.begin();
            auto found_pos = data.begin();
            while (found_pos != data.end()) {
                begin_pos = data.begin();
                found_pos = std::search(begin_pos, data.end(), toSearch.begin(), toSearch.end());
                if (found_pos != data.end()) {
                    preStr = std::string(begin_pos, found_pos);
                    if (replaceStr.empty() && el.second) {
                        replaceStr = *el.second;
                    }
                    else if (replaceStr.empty() && getReplaceString) {
                        replaceStr = getReplaceString(valIdx);
                    }
                    postStr.assign(found_pos + toSearch.size(), data.end());
                    data = preStr + replaceStr + postStr;
                }
            }
            valIdx++;
        }
    }

    /**
     * @brief findAndReplaceAll
     * 
     * @param data 
     * @param values 
     * @param getReplaceString 
     */
    static inline void findAndReplaceAll(std::string &data, const std::vector<std::string> &values, std::function<std::string(int)> getReplaceString) {
        std::vector<std::pair<std::string, std::optional<std::string>>> valuePairs;
        valuePairs.reserve(values.size());
        for (auto val : values) {
            valuePairs.emplace_back(std::make_pair(val, std::nullopt));
        }
        findAndReplaceAll(data, valuePairs, getReplaceString);
    }

    /**
     * @brief findAndReplaceAll
     * 
     * @param data 
     * @param toSearch 
     * @param replaceStr 
     */
    static inline void findAndReplaceAll(std::string &data, const std::string &toSearch, const std::string &replaceStr) {
        findAndReplaceAll(data, {{toSearch, replaceStr}});
    }

    /**
     * @brief stringToLowercase
     * 
     * @param inputString 
     * @return std::string 
     */
    static inline std::string stringToLowercase(const std::string &inputString) {
        std::string outputString;
        std::for_each(inputString.begin(), inputString.end(), [&](char ch) { outputString += std::tolower(ch); });
        return outputString;
    }

    /**
     * @brief floatingPointConverter
     * 
     * @param value 
     * @return std::tuple<uint8_t, uint8_t, uint8_t> 
     */
    static inline std::tuple<uint8_t, uint8_t, uint8_t> floatingPointConverter(float value) {
        int32_t decimal = value * (1 << 20); 

        const uint32_t fractional = decimal & 0xFFFFF;
        decimal >>= 20;

        return std::make_tuple((decimal << 4) | (fractional >> 16), fractional >> 8, fractional & 0xFF);
    }

    static inline uint32_t swapBytes(uint32_t toSwap)
    {
#ifdef __GNUC__
        return __builtin_bswap32(toSwap);
#else
        return ((((toSwap)&0xff000000) >> 24) | (((toSwap)&0x00ff0000) >> 8) | (((toSwap)&0x0000ff00) << 8) |
                (((toSwap)&0x000000ff) << 24));
#endif
    }

    /**
     * @brief generateRandomID
     * 
     * @param length 
     * @return std::string 
     */
    [[nodiscard]] std::string generateRandomId(std::size_t length) noexcept;

    namespace Filesystem {
        /**
         * @brief computeFileCRC32
         * 
         * @param file 
         * @return unsigned long 
         */
        [[nodiscard]] unsigned long computeFileCRC32(std::FILE *file) noexcept;

        /**
         * @brief getline
         * 
         * @param stream 
         * @param length 
         * @return std::string 
         */
        [[nodiscard]] std::string getline(std::FILE *stream, uint32_t length = 1024) noexcept;
    } 
} 