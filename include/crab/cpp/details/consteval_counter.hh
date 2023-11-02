// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <cstdint>

namespace crab::cpp {

namespace details {

template<typename, auto>
struct ADLHelper {
  friend consteval auto adl_helper(ADLHelper, auto...);
};

template<typename InstanceTag, auto x>
struct InjectADLHelperFunctionDef {
  friend consteval auto adl_helper(ADLHelper<InstanceTag, x>, auto...) {
  }
};

}  // namespace details

template<typename InstanceTag>
struct ConstevalCounter {
  template<auto x = 0,
           auto =
               [] {
               }>
  static consteval auto current() {
    if constexpr (requires {
                    adl_helper(details::ADLHelper<InstanceTag, x> {});
                  }) {
      return ConstevalCounter<InstanceTag>::current<x + 1>();
    } else {
      return x;
    }
  }

  template<auto x = 0,
           auto =
               [] {
               }>
  static consteval auto next() {
    if constexpr (requires {
                    adl_helper(details::ADLHelper<InstanceTag, x> {});
                  }) {
      return ConstevalCounter<InstanceTag>::next<x + 1>();
    } else {
      (void)details::InjectADLHelperFunctionDef<InstanceTag, x> {};
    }
    return x;
  }
};

}  // namespace crab::cpp
