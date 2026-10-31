#pragma once
#include "landlockpp/ruleset.hpp"
#include <expected>
#include <filesystem>
#include <functional>
#include <sys/wait.h>
#include <unistd.h>
namespace landlockpp::test {
const std::filesystem::path stage_dir { LANDLOCKPP_TEST_FILES_STAGE_DIR };
class Runner {
public:
  using fn_t = std::function<uint8_t(void)>;
  Runner(Ruleset&& ruleset, fn_t fn) : fn_{fn}, ruleset_{std::move(ruleset)} {}
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
  Ruleset ruleset_;
};

struct Stage {
  Stage() {
    std::filesystem::copy(
      LANDLOCKPP_TEST_FILES_DIR,
      LANDLOCKPP_TEST_FILES_STAGE_DIR,
      std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing
    );
  }

  ~Stage() {
    std::filesystem::remove_all(LANDLOCKPP_TEST_FILES_STAGE_DIR);
  }
};

}
