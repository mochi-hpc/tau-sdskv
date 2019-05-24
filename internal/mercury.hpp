/**
 * @file margo.hpp
 * @author Andrew Gaspar (you@domain.com)
 * @brief C++ interface to mercury
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 *
 */

#ifndef TAUSDSKV_MERCURY_HPP_
#define TAUSDSKV_MERCURY_HPP_

// Third Party Includes
#include <mercury.h>

// STL Includes
#include <exception>
#include <string>

namespace tausdskv {
class MercuryException {
  public:
    explicit MercuryException(hg_return_t status);

  private:
    std::string what_;
    hg_return_t status_;
};

inline void MercuryCheck(hg_return_t status) {
    if (status != HG_SUCCESS) {
        throw MercuryException(status);
    }
}
} // namespace tausdskv

#endif // TAUSDSKV_MERCURY_HPP_