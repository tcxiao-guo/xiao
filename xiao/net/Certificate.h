/**
 * @file Certificate.h
 * @author xiao guo
 * @brief
 * @version 0.1
 * @date 2024-03-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <string>
#include <memory>

namespace xiao
{
    struct Certificate
    {
        virtual ~Certificate() = default;
        virtual std::string sha1Fingerprint() const = 0;
        virtual std::string sha256Fingerprint() const = 0;
        virtual std::string pem() const = 0;
    };
    using CertificatePtr = std::shared_ptr<Certificate>;
} // namespace xiao
