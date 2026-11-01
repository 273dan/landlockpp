# landlockpp
![CI](https://github.com/273d/landlockpp/actions/workflows/ci.yml/badge.svg)

**landlockpp** is an exception-free, header only C++23 wrapper for Linux Landlock v1-7
```cpp
#include "landlockpp/landlockpp.hpp"
using namespace landlockpp;
auto ruleset { 
  RulesetBuilder<8, {.fs = FsAccess::All, .net = NetAccess::All}>{}
  .add_path<FsAccess::ReadDir | FsAccess::Write>("/tmp")
  .add_path<FsAccess::ReadDir>("/usr")
  .add_net_port<NetAccess::BindTCP>(8080)
  .build()
};
if(ruleset.has_value()) {
    ruleset->restrict_self();
} else ...
```
## Features
- Covers filesystem access, net access, scope, and restrict self flags up to Landlock ABI 7
- Exception free api with `std::expected`
- Rulesets validated at compile time

