// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <concepts>
#include <memory>
#include <new>
#include <type_traits>

namespace crab::result::details {

template<typename T>
struct ResultVariantStorage {
  static_assert(std::destructible<T>, "T must be nothrow destructible");

 public:
  using value_type = T;
  using reference  = value_type &;
  using pointer    = value_type *;

  constexpr ResultVariantStorage() noexcept = default;
  ~ResultVariantStorage() noexcept          = default;

  template<typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr reference construct_inner(Args &&...args) noexcept(
      std::is_nothrow_constructible_v<T, Args &&...>) {
    ::new (as_void_ptr()) value_type(std::forward<Args>(args)...);
    return *std::launder(as_ptr());
  }

  template<typename Func, typename FuncR = std::invoke_result_t<Func>>
    requires std::is_nothrow_invocable_v<Func> &&
             std::constructible_from<T, FuncR>
  constexpr reference construct_inner_from(Func &&func) noexcept(
      std::is_nothrow_invocable_v<Func> &&
      std::is_nothrow_constructible_v<T, FuncR>) {
    ::new (as_void_ptr()) value_type(std::forward<Func>(func)());
    return *std::launder(as_ptr());
  }

  pointer as_ptr() noexcept {
    return std::addressof(value_);
  }

 private:
  void *as_void_ptr() noexcept {
    return static_cast<void *>(std::addressof(value_));
  }

 private:
  std::aligned_storage_t<sizeof(T), alignof(T)> value_;
};

}  // namespace crab::result::details
