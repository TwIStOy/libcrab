// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

namespace crab::result::details {

template<typename T, typename E>
class Result {
 public:
 private:
  enum class State {
    IsUninitialized,
    IsOk,
    IsErr,
  };
};

}  // namespace crab::result::details
