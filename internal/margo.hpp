/**
 * @file margo.hpp
 * @author Andrew Gaspar (you@domain.com)
 * @brief C++ interface to margo
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

#ifndef TAUSDSKV_MARGO_HPP_
#define TAUSDSKV_MARGO_HPP_

// Third Party Includes
#include <margo.h>

// STL Includes
#include <exception>
#include <string_view>

namespace tausdskv {
class MargoInstance;

enum class MargoMode { Client = MARGO_CLIENT_MODE, Server = MARGO_SERVER_MODE };

class MargoInitException : public std::exception {
  public:
    char const *what() const noexcept override;

  private:
};

class MargoAddress {
  public:
    explicit MargoAddress(MargoInstance const &mid, hg_addr_t addr);

    MargoAddress(MargoAddress const &other);
    MargoAddress &operator=(MargoAddress const &other);

    MargoAddress(MargoAddress &&other);
    MargoAddress &operator=(MargoAddress &&other);

    ~MargoAddress();

    void Reset();

    hg_addr_t Address() const { return addr_; }

  private:
    MargoInstance const *mid_ = nullptr;
    hg_addr_t addr_ = HG_ADDR_NULL;
};

class MargoInstance {
  public:
    MargoInstance(std::string_view addr_str,
                  MargoMode mode,
                  bool use_progress_thread,
                  int rpc_thread_count);

    MargoInstance(MargoInstance const &) = delete;
    MargoInstance &operator=(MargoInstance const &) = delete;

    MargoInstance(MargoInstance &&other);
    MargoInstance &operator=(MargoInstance &&other);

    ~MargoInstance();

    void Reset();

    margo_instance_id InstanceId() const { return mid_; }

    MargoAddress Lookup(std::string_view name);

  private:
    margo_instance_id mid_ = nullptr;
};
} // namespace tausdskv

#endif // TAUSDSKV_MARGO_HPP_