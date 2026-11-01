#pragma once
#include "landlockpp/ruleset.hpp"
#include <expected>
#include <filesystem>
#include <functional>
#include <sys/wait.h>
#include <unistd.h>
#include "doctest/doctest.h"
namespace landlockpp::test {

const std::filesystem::path files_dir { LANDLOCKPP_TEST_FILES_DIR };
const std::filesystem::path stage_dir { LANDLOCKPP_TEST_FILES_STAGE_DIR };

constexpr int test_abi_ver { 8 };

template <int version, RulesetFlags flags>
class Runner {
public:
  using fn_t = std::function<uint8_t(void)>;
  using Ruleset_t = Ruleset<version, flags>;
  Runner(Ruleset_t&& ruleset, fn_t fn) : fn_{fn}, ruleset_{std::move(ruleset)} {}
  void set_fn(fn_t fn) { fn_ = fn; }
  fn_t::result_type run() {
    pid_t pid{fork()};
    if(pid == 0) {
      auto exp { ruleset_.restrict_self() };
      if( ! exp.has_value()) ::_exit(0);
      uint32_t ec {fn_()};
      ::_exit(ec);
    } else {
      int status;
      ::waitpid(pid, &status, 0);
      return WEXITSTATUS(status);
    }
  }
private:
  fn_t fn_;
  Ruleset_t ruleset_;
};

struct Stage {
  Stage() {
    std::filesystem::copy(
      files_dir,
      stage_dir,
      std::filesystem::copy_options::recursive
      | std::filesystem::copy_options::overwrite_existing
    );
  }

  ~Stage() {
    std::filesystem::remove_all(stage_dir);
  }
};

template <typename T>
inline void require_info(std::string_view source, std::expected<T, std::error_code>& e) {
  if( ! e.has_value() ) {
    FAIL(source, ": ", e.error().message());
  }
    
}



}
