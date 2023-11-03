// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <cstdint>

#include "crab/result/details/storage.hh"

namespace crab::result::details {

template<typename T, typename E>
class Result {
 public:
  bool is_ok() const;

 private:
  enum class State : uint8_t {
    IsUninitialized,
    IsOk,
    IsErr,
  };

  State state_;

  union {
    ResultVariantStorage<T> ok_storage_;
    ResultVariantStorage<E> err_storage_;
  };
};

template<typename T, typename E>
inline bool Result<T, E>::is_ok() const {
  return state_ == State::IsOk;
}

}  // namespace crab::result::details
