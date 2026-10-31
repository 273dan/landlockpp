#pragma once
#include <unistd.h>
#include <utility>
namespace landlockpp::detail {
class UniqueFd {
public:

  [[nodiscard]] bool valid() const { return fd_ >= 0; }
  [[nodiscard]] int get() const { return fd_; }
  [[nodiscard]] int release() { return std::exchange(fd_, -1); }

  UniqueFd() noexcept = default;
  explicit UniqueFd(int fd) : fd_{fd} {}

  UniqueFd(UniqueFd&& other) noexcept {
    if(this != &other) {
      this->fd_ = std::exchange(other.fd_, -1);
    }
  }

  UniqueFd& operator=(UniqueFd&& other) noexcept {
    if(this != &other) {
      if(fd_ >= 0) {
        ::close(fd_);
      }
      this->fd_ = std::exchange(other.fd_, -1);
    }
    return *this;
  }

  ~UniqueFd() {
    if(fd_ >= 0) ::close(fd_);
  }

  UniqueFd(const UniqueFd&) = delete;
  UniqueFd& operator=(const UniqueFd&) = delete;


private:
  int fd_{-1};
};

}
