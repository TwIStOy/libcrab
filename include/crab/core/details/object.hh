// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "crab/cpp/details/consteval_counter.hh"

namespace crab::core {

template<typename T, typename InstanceTag>
class Object {
 public:
  using instance_tag  = InstanceTag;
  using value_type    = T;
  using pointer       = std::allocator_traits<std::allocator<T>>::pointer;
  using const_pointer = std::allocator_traits<std::allocator<T>>::const_pointer;

  Object(std::convertible_to<T> auto &&x)
      : inner_ {std::forward<decltype(x)>(x)} {
  }

  Object(const Object &)          = delete;
  auto &operator=(const Object &) = delete;
  Object(Object &&)               = delete;
  auto &operator=(Object &&)      = delete;

 private:
  T inner_;

  template<typename U, std::size_t, typename, typename>
  friend auto Move(U &&value);
};

template<typename T, typename InstanceTag = decltype([] {
                     })>
Object(T &&) -> Object<std::decay_t<T>, InstanceTag>;

template<
    typename U,
    std::size_t x = cpp::NextCounter<typename std::decay_t<U>::instance_tag>(),
    typename      = decltype([] {
    }),
    typename      = std::enable_if_t<x == 0>>
auto Move(U &&value) {
  using object_t         = std::decay_t<U>;
  using value_type       = typename object_t::value_type;
  using new_instance_tag = decltype([] {
  });
  return Object<value_type, new_instance_tag>(std::move(value.inner_));
}

}  // namespace crab::core

namespace __compile_testing {

using namespace crab::core;

inline auto test_object() {
  auto x  = Object {42};
  auto y  = Move(x);
  auto y2 = Move(y);
  (void)y2;

  using counter_tag = decltype([] {
  });
  static_assert(crab::cpp::NextCounter<counter_tag>() == 0);
  static_assert(crab::cpp::NextCounter<counter_tag>() == 1);
}

}  // namespace __compile_testing
