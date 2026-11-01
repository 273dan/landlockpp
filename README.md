# landlockpp
**landlockpp** is an execption-free, header only C++23 wrapper for Linux Landlock v1-7
```cpp
#include "landlockpp/landlockpp.hpp"
using namespace landlockpp;
auto ruleset { 
  RulesetBuilder<8, {.fs = FsAccess::All, .net = NetAccess::All}>{}
  .add_path<FsAccess::ReadDir | FsAccess::Write>("/tmp")
  .add_path<FsAccess::ReadDir>("/usr")
  .build()
};
if(ruleset.has_value()) {
    ruleset->restrict_self();
}
}
```
## Features
- Covers filesystem access, net access, scope, and restrict self flags up to Landlock ABI 7
- Exception free api with `std::expected`
- Rulesets validated at compile time

