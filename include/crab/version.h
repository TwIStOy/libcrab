// Copyright (c) 2020 - present, Hawtian Wang (twistoy.wang@gmail.com)
//

#pragma once  // NOLINT(build/header_guard)

#define CRAB_VERSION_MAJOR 0
#define CRAB_VERSION_MINOR 1
#define CRAB_VERSION_PATCH 0

#define _VERSION_MK_STR(x) #x
#define _VERSION_MK_VER(x, y, z)                                               \
  _VERSION_MK_STR(x) "." _VERSION_MK_STR(y) "." _VERSION_MK_STR(z)

#define CRAB_VERSION                                                           \
  _VERSION_MK_STR(CRAB_VERSION_MAJOR, CRAB_VERSION_MINOR, CRAB_VERSION_PATCH)
