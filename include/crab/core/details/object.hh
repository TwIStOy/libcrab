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
  using instance_counter = cpp::ConstevalCounter<InstanceTag>;
  using value_type       = T;
  using pointer          = std::allocator_traits<std::allocator<T>>::pointer;
  using const_pointer = std::allocator_traits<std::allocator<T>>::const_pointer;

  Object(std::convertible_to<T> auto &&x)
      : inner_ {std::forward<decltype(x)>(x)} {
  }

  Object(const Object &)          = delete;
  auto &operator=(const Object &) = delete;
  Object(Object &&)               = delete;
  auto &operator=(Object &&)      = delete;

  template<typename Tag>
  auto Move()
    requires(instance_counter::next() == 0)
  {
    return Object<value_type, Tag>(std::move(inner_));
  }

 private:
  T inner_;

  template<typename U, typename Tag>
  friend auto Move(U &&value);
};

template<typename T, typename InstanceTag = decltype([] {
                     })>
Object(T &&) -> Object<std::decay_t<T>, InstanceTag>;

template<typename U, typename Tag = decltype([] {
                     })>
auto Move(U &&value)
  requires(std::remove_cvref_t<U>::instance_counter::next() == 0)
{
  return value.template Move<Tag>();
}

}  // namespace crab::core

namespace __compile_testing {

using namespace crab::core;

inline auto test_object() {
  auto x  = Object {42};
  auto y  = Move(x);
  auto y2 = Move(x);

  using counter = crab::cpp::ConstevalCounter<decltype([] {
  })>;
  static_assert(counter::next() == 0);
  static_assert(counter::next() == 1);
}

}  // namespace __compile_testing
