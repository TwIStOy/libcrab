// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <concepts>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace crab::result::details {

template<typename T>
struct ResultVariantStorage {
  static_assert(std::destructible<T>, "T must be nothrow destructible");

 public:
  using value_type = T;

 private:
  using allocator_type = std::allocator<T>;

 public:
  using reference       = value_type &;
  using const_reference = const value_type &;
  using pointer         = std::allocator_traits<allocator_type>::pointer;
  using const_pointer   = std::allocator_traits<allocator_type>::const_pointer;

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

  void destruct_inner() noexcept {
    std::destroy_at(as_ptr());
  }

  reference as_ref() & noexcept {
    return *as_ptr();
  }

  const_reference as_ref() const & noexcept {
    return *as_ptr();
  }

  value_type &&as_Ref() && noexcept {
    return std::move(*as_ptr());
  }

 private:
  void *as_void_ptr() noexcept {
    return static_cast<void *>(std::addressof(value_));
  }

  pointer as_ptr() noexcept {
    return std::launder(std::addressof(value_));
  }

  const_pointer as_ptr() const noexcept {
    return std::launder(std::addressof(value_));
  }

 private:
  std::aligned_storage_t<sizeof(T), alignof(T)> value_;
};

template<typename T>
struct ResultVariantStorage<T &> {
 public:
  using value_type = T &;
  using reference  = T &;

  ResultVariantStorage() noexcept = default;

  reference construct_inner(T &value) noexcept {
    value_ = std::addressof(value);
    return value;
  }

  template<typename Func, typename FuncR = std::invoke_result_t<Func>>
    requires std::is_nothrow_invocable_v<Func> &&
             (std::is_pointer_v<FuncR>
                  ? std::same_as<std::remove_pointer_t<FuncR>, T>
                  : std::same_as<FuncR, T &>)
  constexpr reference construct_inner_from(Func &&func) noexcept(
      std::is_nothrow_invocable_v<Func> &&
      std::is_nothrow_constructible_v<T, FuncR>) {
    if constexpr (std::is_pointer_v<FuncR>) {
      value_ = std::forward<Func>(func)();
    } else {
      value_ = std::addressof(std::forward<Func>(func)());
    }
    return *value_;
  }

  void destruct_inner() noexcept {
    // destruct do nothing
  }

  reference as_ref() noexcept {
    return *value_;
  }

 private:
  T *value_ = nullptr;
};

template<>
struct ResultVariantStorage<void> {
 public:
  ResultVariantStorage() noexcept = default;

  void construct_inner() noexcept {
  }

  template<typename Func>
    requires std::is_void_v<std::invoke_result_t<Func>>
  void construct_inner_from(Func &&func) noexcept(
      std::is_nothrow_invocable_v<Func>) {
    std::forward<Func>(func)();
  }

  void destruct_inner() noexcept {
  }

  void as_ref() const noexcept {
  }
};

template<>
class ResultVariantStorage<void const> : public ResultVariantStorage<void> {};

}  // namespace crab::result::details
