/**
 * @file margo.hpp
 * @author Andrew Gaspar (agaspar@lanl.gov)
 * @brief C++ interface to mercury
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019 Triad National Security, LLC
 */

#ifndef TAU_SDSKEYVAL_MERCURY_HPP_
#define TAU_SDSKEYVAL_MERCURY_HPP_

// Third Party Includes
#include <mercury.h>

// STL Includes
#include <exception>
#include <string>

namespace tau_sdskeyval {
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
} // namespace tau_sdskeyval

#endif // TAU_SDSKEYVAL_MERCURY_HPP_