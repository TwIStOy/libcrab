// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <type_traits>
#include <utility>

#include "crab/cpp/details/consteval_counter.hh"
#include "crab/cpp/details/macros.hh"

namespace crab::ops::details {

template<
    typename T,
    std::size_t x = cpp::NextCounter<typename std::decay_t<T>::instance_tag>(),
    typename      = CRAB_ALWAYS_NEW_TYPE,
    typename      = std::enable_if_t<x == 0>>
constexpr auto InvokeFnOnce(T &&func, auto &&...args) {
  std::move(func.func_)(std::forward<decltype(args)>(args)...);
}

template<typename F, typename InstTag>
struct FnOnce {
 public:
  using instance_tag = InstTag;

  constexpr FnOnce(auto &&func)
      : func_(std::forward<decltype(func)>(func)) {
  }

  constexpr auto operator()(auto &&...args) {
    return InvokeFnOnce(*this, std::forward<decltype(args)>(args)...);
  }

 private:
  [[no_unique_address]] F func_;

  template<typename U, std::size_t, typename, typename>
  friend constexpr auto InvokeFnOnce(U &&value, auto &&...);
};

template<typename F, typename InstTag = CRAB_ALWAYS_NEW_TYPE>
FnOnce(F &&func) -> FnOnce<std::remove_cvref_t<F>, InstTag>;

}  // namespace crab::ops::details

namespace crab::ops {

using details::FnOnce;

}  // namespace crab::ops

namespace __compile_testing {

inline auto test_fn_once() {
  using namespace crab::ops;
  auto f = FnOnce([](int x) {
    return x + 1;
  });
  f(1);
  f(1);
}

}  // namespace __compile_testing
