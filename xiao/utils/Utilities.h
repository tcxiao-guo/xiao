/**
 * @file Utilities.h
 * @author Xiao Guo
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <xiao/exports.h>
#include <string>

namespace xiao
{
    /**
     * @brief Xiao helper functions.
     *
     */
    namespace utils
    {
        /**
         * @brief Convert a wide string to a UTF-8.
         * @details UCS2 on Windows, UTF-32 on Linux & Mac
         *
         * @param wstr String to convert
         *
         * @return converted string.
         */
        XIAO_EXPORT std::string toUtf8(const std::wstring &wstr);
        /**
         * @brief Convert a UTF8 string to a wide string.
         * @details UCS2 on Windows, UTF-32 on Linux & Mac
         *
         * @param str String to convert
         *
         * @return converted string.
         */
        XIAO_EXPORT std::wstring fromUtf8(const std::string &str);

        /**
         * @brief Convert a wid string path with arbitrary directory separators
         * to a UTF-8 protable path for use with xiao.
         *
         * This is a helper, mainly for Windows and multi-platform projects.
         *
         * @note On Windows, backslash directory separators are converted to slash to
         * keep portable paths.
         *
         * @remarks On other OSes, backslashes are not converted to slash, since they
         * are valid characters for directory/file names.
         *
         * @param strPath Wide string path.
         *
         * @return std::string UTF-8 path, with slash directory separator.
         *
         */
        XIAO_EXPORT std::string fromWidePath(const std::wstring &strPath);
        /**
         * @brief Convert a UTF-8 path with arbitrary directory separator to a wide
         * string path.
         *
         * This is a helper, mainly for Windows and multi-platform projects.
         * @note On Windows, slash directory separators are converted to backslash.
         * Although it accepts both slash and backslash as directory separator in its
         * API, it is better to stick to its standard.

         * @remarks On other OSes, slashes are not converted to backslashes, since they
         * are not interpreted as directory separators and are valid characters for
         * directory/file names.
         *
         * @param strUtf8Path Ascii path considered as being UTF-8.
         *
         * @return std::wstring path with, on windows, standard backslash directory
         * separator to stick to its standard.
         */
        XIAO_EXPORT std::wstring toWidePath(const std::string &strUtf8Path);

#if defined(_WIN32) && !defined(__MINGW32__)
        /**
         * @details Convert an UTF-8 path to a native path.
         *
         * This is a helper for Windows and multi-platform projects.
         *
         * Converts the path from UTF-8 to wide string and replaces slash directory
         * separators with backslash.
         *
         * @remarks Although it accepts both slash and backslash as directory
         * separators in its API, it is better to stick to its standard.
         *
         * @param strPath UTF-8 path.
         *
         * @return Native path suitable for the Windows unicode API.
         */
        inline std::wstring toNativePath(const std::string &strPath)
        {
            return toWidePath(strPath);
        }
        /**
         * @details Convert an UTF-8 path to a native path.
         *
         * This is a helper for non-Windows OSes for multi-platform projects.
         *
         * Does nothing.
         *
         * @param strPath UTF-8 path.
         *
         * @return \p strPath.
         */
        inline const std::wstring &toNativePath(const std::wstring &strPath)
        {
            return strPath;
        }
#else
        /**
         * @details Convert an UTF-8 path to a native path.
         *
         * This is a helper for non-Windows OSes for multi-platform projects.
         *
         * Does nothing.
         *
         * @param strPath UTF-8 path.
         *
         * @return \p strPath.
         */
        inline const std::string &toNativePath(const std::string &strPath)
        {
            return strPath;
        }
        /**
         * @details Convert an wide string path to a UTF-8 path.
         *
         * This is a helper, mainly for Windows and multi-platform projects.
         *
         * @note On Windows, backslash directory separators are converted to slash to
         * keep portable paths.

        * @warning On other OSes, backslashes are not converted to slash, since they
        * are valid characters for directory/file names.
        *
        * @param strPath wide string path.
        *
        * @return Generic path, with slash directory separators
        */
        inline std::string toNativePath(const std::wstring &strPath)
        {
            return fromWidePath(strPath);
        }
#endif
        /**
         * @note NoOP on all OSes
         *
         */
        inline const std::string &fromNativePath(const std::string &strPath)
        {
            return strPath;
        }
        /**
         * @note fromWidePath() on all OSes
         *
         */
        inline std::string fromNativePath(const std::wstring &strPath)
        {
            return fromWidePath(strPath);
        }

        /**
         * @brief Check if the name supplied by the SSL Cert maths a FQDN
         * @param certName The name supplied by the SSL Cert
         * @param hostName The FQDN to match
         *
         * @return true if mathes. false otherwise
         *
         */
        bool verifySslName(const std::string &certName, const std::string &hostName);

        /**
         * @brief Returns the TLS backend used by xiao. Could be "None", "OpenSSL" or "Botan"
         *
         */
        XIAO_EXPORT std::string tlsBackend();

        struct Hash128
        {
            uint8_t bytes[16];
        };
        struct Hash160
        {
            uint8_t bytes[20];
        };
        struct Hash256
        {
            uint8_t bytes[32];
        };

        /**
         * @brief Compute the MD5 hash of the given data
         * @note don't use MD5 for new applications. It's here only for compatibility
         *
         */
        XIAO_EXPORT Hash128 md5(const void *data, size_t len);
        inline Hash128 md5(const std::string &str)
        {
            return md5(str.data(), str.size());
        }

        /**
         * @brief Compute the SHA1 hash of the given data
         *
         */
        XIAO_EXPORT Hash160 sha1(const void *data, size_t len);
        inline Hash160 sha1(const std::string &str)
        {
            return sha1(str.data(), str.size());
        }

        /**
         * @brief Compute the SHA256 hash of the given data
         */
        XIAO_EXPORT Hash256 sha256(const void *data, size_t len);
        inline Hash256 sha256(const std::string &str)
        {
            return sha256(str.data(), str.size());
        }

        /**
         * @brief Compute the SHA3 hash of the given data
         */
        XIAO_EXPORT Hash256 sha3(const void *data, size_t len);
        inline Hash256 sha3(const std::string &str)
        {
            return sha3(str.data(), str.size());
        }

        /**
         * @brief Compute the BLAKE2b hash of the given data
         * @note When in doubt, use SHA3 or BLAKE2b. Both are safe and SHA3 is faster if
         * you are using OpenSSL and it has SHA3 in hardware mode. Owtherwise BLAKE2b is
         * faster in software.
         */
        XIAO_EXPORT Hash256 blake2b(const void *data, size_t len);
        inline Hash256 blake2b(const std::string &str)
        {
            return blake2b(str.data(), str.size());
        }

        /**
         * @brief hex encode the given data
         * @note When in doubt, use SHA3 or BLAKE2b. Both are safe and SHA3 is faster if
         * you are using OpenSSL and it has SHA3 in hardware mode. Owtherwise BLAKE2b is
         * faster in software.
         */
        XIAO_EXPORT std::string toHexString(const void *data, size_t len);
        inline std::string toHexString(const Hash128 &hash)
        {
            return toHexString(hash.bytes, sizeof(hash.bytes));
        }
        inline std::string toHexString(const Hash160 &hash)
        {
            return toHexString(hash.bytes, sizeof(hash.bytes));
        }

        inline std::string toHexString(const Hash256 &hash)
        {
            return toHexString(hash.bytes, sizeof(hash.bytes));
        }

        /**
         * @brief Generates cryptographically secure random bytes
         * @param ptr Pointer to the buffer to fill
         * @param size Size of the buffer
         * @return true if successful, false otherwise
         *
         * @note This function really sholdn't fail, but it's possible that
         *
         *   - OpenSSL can't access /dev/urandom
         *   - Compiled with glibc that supports getentropy() but the kernel doesn't
         *
         * When using Botan or on *BSD/macOS, this function will always succeed.
         */
        XIAO_EXPORT bool secureRandomBytes(void *ptr, size_t size);

    } // namespace utils
}