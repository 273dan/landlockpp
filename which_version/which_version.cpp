#include "landlockpp/landlockpp.hpp"
#include <print>
int main() {
  auto ver { landlockpp::abi_version() };
  if(ver.has_value()) std::println("{}",  ver.value());
  else std::println("{}", "landlock not supported");


}
