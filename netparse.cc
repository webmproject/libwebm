// Copyright (c) 2012 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

//#include <cstdio>
//#include <cstdlib>
#include "./mkvparser.hpp"
#include "./mkvstream.h"

namespace {
  int ParserEbmlHeader(long long* pos);
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    fprintf(stdout, "usage: netparse <mkvfile>\n");
    return EXIT_SUCCESS;
  }

  MkvStream reader;
  const char* const filename = argv[1];

  int e = reader.Open(filename);
  if (e) {
    fprintf(stdout, "open failed\n");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}

namespace {

int ParserEbmlHeader(long long* pos) {
}

}
