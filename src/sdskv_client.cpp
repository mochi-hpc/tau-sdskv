/**
 * @file sdskv_client.cpp
 * @author Andrew Gaspar (you@domain.com)
 * @brief Implementation of SDSKV C++
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

// Self Include
#include <sdskv_client.hpp>

// STL Includes
#include <iostream>
#include <sstream>
#include <string>

using namespace tausdskv;

SdskvException::SdskvException(int status) : status_(status) {
    std::stringstream ss;
    ss << "SdskvException: ";
#define CASE(enum)                                                                                 \
    case enum:                                                                                     \
        ss << #enum;                                                                               \
        break;

    switch (status) {
        CASE(SDSKV_ERR_ALLOCATION)
        CASE(SDSKV_ERR_INVALID_ARG)
        CASE(SDSKV_ERR_MERCURY)
        CASE(SDSKV_ERR_DB_CREATE)
        CASE(SDSKV_ERR_DB_NAME)
        CASE(SDSKV_ERR_UNKNOWN_DB)
        CASE(SDSKV_ERR_UNKNOWN_PR)
        CASE(SDSKV_ERR_PUT)
        CASE(SDSKV_ERR_UNKNOWN_KEY)
        CASE(SDSKV_ERR_SIZE)
        CASE(SDSKV_ERR_ERASE)
        CASE(SDSKV_ERR_MIGRATION)
        CASE(SDSKV_OP_NOT_IMPL)
        CASE(SDSKV_ERR_COMP_FUNC)
        CASE(SDSKV_ERR_REMI)
        CASE(SDSKV_ERR_ARGOBOTS)
    default:
        ss << "Unknown(" << status << ")";
        break;
    }

#undef CASE

    what_ = ss.str();
}

const char *SdskvException::what() const noexcept { return what_.c_str(); }

SdskvProviderHandle::SdskvProviderHandle(sdskv_provider_handle_t kvph) : kvph_(kvph) {}

SdskvProviderHandle::SdskvProviderHandle(SdskvProviderHandle const &other) {
    kvph_ = other.kvph_;
    SdskvCheck(sdskv_provider_handle_ref_incr(other.kvph_));
}

SdskvProviderHandle &SdskvProviderHandle::operator=(SdskvProviderHandle const &other) {
    Reset();

    kvph_ = other.kvph_;
    SdskvCheck(sdskv_provider_handle_ref_incr(other.kvph_));

    return *this;
}

SdskvProviderHandle::SdskvProviderHandle(SdskvProviderHandle &&other) {
    std::swap(kvph_, other.kvph_);
}

SdskvProviderHandle &SdskvProviderHandle::operator=(SdskvProviderHandle &&other) {
    Reset();

    std::swap(kvph_, other.kvph_);

    return *this;
}

SdskvProviderHandle::~SdskvProviderHandle() { Reset(); }

void SdskvProviderHandle::Reset() {
    if (kvph_ != SDSKV_PROVIDER_HANDLE_NULL) {
        SdskvCheck(sdskv_provider_handle_release(kvph_));
        kvph_ = SDSKV_PROVIDER_HANDLE_NULL;
    }
}

sdskv_database_id_t SdskvProviderHandle::Open(std::string_view db_name) const {
    std::string db_name_z(db_name);

    sdskv_database_id_t db;
    SdskvCheck(sdskv_open(kvph_, db_name_z.c_str(), &db));

    return db;
}

SdskvClient::SdskvClient(MargoInstance const &mi) {
    SdskvCheck(sdskv_client_init(mi.InstanceId(), &client_));
}

SdskvClient::SdskvClient(SdskvClient &&other) { std::swap(client_, other.client_); }

SdskvClient &SdskvClient::operator=(SdskvClient &&other) {
    Reset();
    std::swap(client_, other.client_);
    return *this;
}

SdskvClient::~SdskvClient() { Reset(); }

void SdskvClient::Reset() {
    if (client_ != SDSKV_CLIENT_NULL) {
        SdskvCheck(sdskv_client_finalize(client_));
        client_ = SDSKV_CLIENT_NULL;
    }
}

SdskvProviderHandle SdskvClient::ProviderHandleCreate(MargoAddress const &address,
                                                      uint16_t provider_id) const {
    sdskv_provider_handle_t kvph;
    SdskvCheck(sdskv_provider_handle_create(client_, address.Address(), provider_id, &kvph));

    return SdskvProviderHandle(kvph);
}