// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#include <type_traits>
#include <utility>

namespace crab::core::cpo {

namespace __tag_invoke_impl {  // NOLINT
void tag_invoke();

struct Fn {
  template<typename CPO,
           typename... Args,
           bool noexcept_ = noexcept(tag_invoke(std::declval<CPO>(),
                                                std::declval<Args>()...))>
  constexpr auto operator()(CPO cpo, Args &&...args) const noexcept(noexcept_) {
    return tag_invoke(std::move(cpo), std::forward<Args>(args)...);
  }
};

}  // namespace __tag_invoke_impl

namespace _tag_invoke {                                // NOLINT
inline constexpr __tag_invoke_impl::Fn tag_invoke {};  // NOLINT
}  // namespace _tag_invoke

using namespace _tag_invoke;  // NOLINT

template<auto &CPO>
using tag_t = std::remove_cvref_t<decltype(CPO)>;

template<typename Tag, typename... Args>
concept tag_invocable = std::is_invocable_v<decltype(tag_invoke), Tag, Args...>;

template<class Tag, class... Args>
concept nothrow_tag_invocable =
    tag_invocable<Tag, Args...> &&
    std::is_nothrow_invocable_v<decltype(tag_invoke), Tag, Args...>;

template<class Tag, class... Args>
inline bool constexpr nothrow_tag_invocable_v =  // NOLINT
    tag_invocable<Tag, Args...> &&
    std::is_nothrow_invocable_v<decltype(tag_invoke), Tag, Args...>;

template<class Tag, class... Args>
using tag_invoke_result =
    std::invoke_result<decltype(tag_invoke), Tag, Args...>;

template<class Tag, class... Args>
using tag_invoke_result_t =
    std::invoke_result_t<decltype(tag_invoke), Tag, Args...>;

}  // namespace crab::core::cpo

namespace crab::core {

using cpo::nothrow_tag_invocable;
using cpo::nothrow_tag_invocable_v;
using cpo::tag_invocable;
using cpo::tag_invoke;
using cpo::tag_invoke_result_t;
using cpo::tag_t;

}  // namespace crab::core
