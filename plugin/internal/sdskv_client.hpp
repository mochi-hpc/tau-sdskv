/**
 * @file sdskeyval_client.hpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief C++ interface to sdskv
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

#ifndef TAU_SDSKEYVAL_SDSKV_CLIENT_HPP_
#define TAU_SDSKEYVAL_SDSKV_CLIENT_HPP_

// STL Includes
#include <exception>
#include <string>

// Third Party Includes
#include <sdskv-client.h>

// Internal Includes
#include <margo.hpp>

namespace tau_sdskeyval {
class SdskvException : public std::exception {
  public:
    explicit SdskvException(int status);
    int Status() const { return status_; }

    const char *what() const noexcept override;

  private:
    std::string what_;
    int status_;
};

inline void SdskvCheck(int status) {
    if (status != SDSKV_SUCCESS) {
        throw SdskvException(status);
    }
}

class SdskvProviderHandle {
  public:
    explicit SdskvProviderHandle(sdskv_provider_handle_t kvph);

    SdskvProviderHandle(SdskvProviderHandle const &other);
    SdskvProviderHandle &operator=(SdskvProviderHandle const &other);

    SdskvProviderHandle(SdskvProviderHandle &&other);
    SdskvProviderHandle &operator=(SdskvProviderHandle &&other);

    ~SdskvProviderHandle();

    void Reset();

    sdskv_database_id_t Open(std::string_view db_name) const;

    void PutMulti(sdskv_database_id_t id,
                  size_t num,
                  const void *const *keys,
                  hg_size_t const *key_sizes,
                  const void *const *values,
                  hg_size_t const *value_sizes) const;

  private:
    sdskv_provider_handle_t kvph_ = SDSKV_PROVIDER_HANDLE_NULL;
};

class SdskvClient {
  public:
    SdskvClient(MargoInstance const &mi);

    SdskvClient(SdskvClient const &) = delete;
    SdskvClient &operator=(SdskvClient const &) = delete;

    SdskvClient(SdskvClient &&other);
    SdskvClient &operator=(SdskvClient &&other);

    ~SdskvClient();

    void Reset();

    SdskvProviderHandle ProviderHandleCreate(MargoAddress const &address,
                                             uint16_t provider_id) const;

  private:
    sdskv_client_t client_ = SDSKV_CLIENT_NULL;
};
} // namespace tau_sdskeyval

#endif // TAU_SDSKEYVAL_SDSKV_CLIENT_HPP_