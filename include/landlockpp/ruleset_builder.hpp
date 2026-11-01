#pragma once
#include "landlockpp/ruleset.hpp"
#include "landlockpp/ruleset_flags.hpp"
#include <expected>
#include <system_error>
namespace landlockpp {
  template <int version, RulesetFlags flags = {}>
  class RulesetBuilder {
  private:
    using RulesetT = Ruleset<version, flags>;
    std::expected<RulesetT, std::error_code> state_;

  public:

    RulesetBuilder() requires requires() { RulesetT::create(); }: state_{RulesetT::create()} {}

    template <FsAccess access>
    RulesetBuilder& add_path(const std::filesystem::path& path)
    noexcept
    requires requires(RulesetT t) { t.template add_path<access>(path); } {
      if( ! state_.has_value()) return *this;
      auto e { state_->template add_path<access>(path) };
      if( ! e.has_value()) state_ = std::unexpected{e.error()};
      return *this;
    }

    template <NetAccess access>
    RulesetBuilder& add_net_port(uint16_t port)
    noexcept
    requires requires(RulesetT t) { t.template add_net_port<access>(port); } {
      if( ! state_.has_value()) return *this;
      auto e { state_->template add_net_port<access>(port) };
      if( ! e.has_value()) state_ = std::unexpected{e.error()};
      return *this;
    }

    template <RestrictSelf rflags = RestrictSelf::None>
    [[nodiscard]] std::expected<void, std::error_code> restrict_self(bool no_new_privs = true)
    noexcept
    requires requires(RulesetT t) { t.template restrict_self<rflags>(true); } {
      if( ! state_.has_value()) return std::unexpected{std::move(state_.error())};
      auto e { state_->template restrict_self<rflags>(no_new_privs) };
      if( ! e.has_value()) return std::unexpected{std::move(e.error())};
      return {};
    }

    [[nodiscard]] decltype(state_) build() {
      return std::move(state_);
    }

  };

}
