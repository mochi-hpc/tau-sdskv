/**
 * @file mercury.cpp
 * @author Andrew Gaspar (you@domain.com)
 * @brief
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

// Self Include
#include <mercury.hpp>

// STL Includes
#include <sstream>

using namespace tausdskv;

MercuryException::MercuryException(hg_return_t status) : status_(status) {
    std::stringstream ss;
    ss << "MercuryException: ";
#define CASE(enum)                                                                                 \
    case enum:                                                                                     \
        ss << #enum;                                                                               \
        break;

    switch (status) {
        CASE(HG_NA_ERROR)
        CASE(HG_TIMEOUT)
        CASE(HG_INVALID_PARAM)
        CASE(HG_SIZE_ERROR)
        CASE(HG_NOMEM_ERROR)
        CASE(HG_PROTOCOL_ERROR)
        CASE(HG_NO_MATCH)
        CASE(HG_CHECKSUM_ERROR)
        CASE(HG_CANCELED)
        CASE(HG_OTHER_ERROR)
    default:
        ss << "Unknown(" << status << ")";
        break;
    }

#undef CASE

    what_ = ss.str();
}