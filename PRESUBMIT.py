# Copyright (c) 2021, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#
#   * Neither the name of Google nor the names of its contributors may
#     be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""Top-level presubmit script for libwebm.

See https://dev.chromium.org/developers/how-tos/depottools/presubmit-scripts for
details on the presubmit API built into depot_tools.
"""
USE_PYTHON3 = True
_INCLUDE_SOURCE_FILES_ONLY = [r".*\.(c|cc|[hc]pp|h)$"]
_LIBWEBM_MAX_LINE_LENGTH = 80


def _CheckChangeLintsClean(input_api, output_api):
  """Makes sure that libwebm/ code is cpplint clean."""
  sources = lambda x: input_api.FilterSourceFile(
      x, files_to_check=_INCLUDE_SOURCE_FILES_ONLY, files_to_skip=None)
  return input_api.canned_checks.CheckChangeLintsClean(input_api, output_api,
                                                       sources)


def _CommonChecks(input_api, output_api):
  results = []
  results.extend(
      input_api.canned_checks.CheckChangeHasNoCrAndHasOnlyOneEol(
          input_api, output_api))
  results.extend(
      input_api.canned_checks.CheckChangeHasNoTabs(input_api, output_api))
  results.extend(
      input_api.canned_checks.CheckChangeHasNoStrayWhitespace(
          input_api, output_api))
  results.extend(
      input_api.canned_checks.CheckLongLines(
          input_api, output_api, maxlen=_LIBWEBM_MAX_LINE_LENGTH))
  results.extend(
      input_api.canned_checks.CheckPatchFormatted(
          input_api,
          output_api,
          check_clang_format=True,
          check_python=True,
          result_factory=output_api.PresubmitError))
  return results


def CheckChangeOnUpload(input_api, output_api):
  results = []
  results.extend(_CommonChecks(input_api, output_api))
  results.extend(_CheckChangeLintsClean(input_api, output_api))
  return results


def CheckChangeOnCommit(input_api, output_api):
  results = []
  results.extend(_CommonChecks(input_api, output_api))
  results.extend(_CheckChangeLintsClean(input_api, output_api))
  return results
