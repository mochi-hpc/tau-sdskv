/**
 * @file margo.cpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief C++ Interface to Margo implementation
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

// Self Include
#include <margo.hpp>

// STL Includes
#include <iostream>
#include <string>
#include <utility>

// Internal Includes
#include <mercury.hpp>

using namespace tau_sdskeyval;

char const *MargoInitException::what() const noexcept { return "Margo failed to initialize!"; }

MargoAddress::MargoAddress(MargoInstance const &mid, hg_addr_t addr) : mid_(&mid), addr_(addr) {}

MargoAddress::MargoAddress(MargoAddress const &other) {
    mid_ = other.mid_;
    MercuryCheck(margo_addr_dup(other.mid_->InstanceId(), other.addr_, &addr_));
}

MargoAddress &MargoAddress::operator=(MargoAddress const &other) {
    Reset();

    mid_ = other.mid_;
    MercuryCheck(margo_addr_dup(other.mid_->InstanceId(), other.addr_, &addr_));

    return *this;
}

MargoAddress::MargoAddress(MargoAddress &&other) {
    mid_ = other.mid_;
    std::swap(addr_, other.addr_);
}

MargoAddress &MargoAddress::operator=(MargoAddress &&other) {
    Reset();

    mid_ = other.mid_;
    std::swap(addr_, other.addr_);

    return *this;
}

MargoAddress::~MargoAddress() { Reset(); }

void MargoAddress::Reset() {
    if (addr_ != HG_ADDR_NULL) {
        MercuryCheck(margo_addr_free(mid_->InstanceId(), addr_));
        addr_ = HG_ADDR_NULL;
    }
}

MargoInstance::MargoInstance(std::string_view addr_str,
                             MargoMode mode,
                             bool use_progress_thread,
                             int rpc_thread_count) {
    std::string addr_str_z(addr_str);

    mid_ = margo_init(
        addr_str_z.c_str(), static_cast<int>(mode), use_progress_thread, rpc_thread_count);
    if (mid_ == MARGO_INSTANCE_NULL) {
        throw MargoInitException();
    }
}

MargoInstance::MargoInstance(MargoInstance &&other) { std::swap(mid_, other.mid_); }

MargoInstance &MargoInstance::operator=(MargoInstance &&other) {
    Reset();
    std::swap(mid_, other.mid_);
    return *this;
}

MargoInstance::~MargoInstance() { Reset(); }

void MargoInstance::Reset() {
    if (mid_ != MARGO_INSTANCE_NULL) {
        margo_finalize(mid_);
        mid_ = MARGO_INSTANCE_NULL;
    }
}

MargoAddress MargoInstance::Lookup(std::string_view name) {
    std::string name_z(name);

    hg_addr_t addr;
    MercuryCheck(margo_addr_lookup(InstanceId(), name_z.c_str(), &addr));

    return MargoAddress(*this, addr);
}