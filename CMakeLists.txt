# Copyright (c) 2015 The WebM project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license that can be found
# in the LICENSE file in the root of the source tree. An additional intellectual
# property rights grant can be found in the file PATENTS.  All contributing
# project authors may be found in the AUTHORS file in the root of the source
# tree.
cmake_minimum_required(VERSION 3.2)
project(LIBWEBM CXX)

include(GNUInstallDirs)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flags.cmake")

if(NOT BUILD_SHARED_LIBS)
  include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc_runtime.cmake")
endif()

# Build/test configuration flags.
option(WEBM_ENABLE_WEBMTS "Enables WebM PES/TS support." ON)
option(WEBM_ENABLE_WEBMINFO "Enables building webm_info." ON)
option(WEBM_ENABLE_TESTS "Enables tests." OFF)

if(WIN32
   OR CYGWIN
   OR MSYS)
  # Allow use of rand_r() / fdopen() and other POSIX functions.
  require_cxx_flag_nomsvc("-std=gnu++11")
else()
  require_cxx_flag_nomsvc("-std=c++11")
endif()

add_cxx_preproc_definition("__STDC_CONSTANT_MACROS")
add_cxx_preproc_definition("__STDC_FORMAT_MACROS")
add_cxx_preproc_definition("__STDC_LIMIT_MACROS")

# Set up compiler flags and build properties.

if(MSVC)
  add_cxx_flag_if_supported("/W4")
  # Disable MSVC warnings that suggest making code non-portable.
  add_cxx_flag_if_supported("/wd4996")
  if(ENABLE_WERROR)
    add_cxx_flag_if_supported("/WX")
  endif()
else()
  add_cxx_flag_if_supported("-Wall")
  add_cxx_flag_if_supported("-Wextra")
  add_cxx_flag_if_supported("-Wnarrowing")
  add_cxx_flag_if_supported("-Wno-deprecated")
  add_cxx_flag_if_supported("-Wshorten-64-to-32")
endif()

set(LIBWEBM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIBWEBM_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

# Source list variables.
set(dumpvtt_sources "${LIBWEBM_SRC_DIR}/webm_parser/dumpvtt.cc")

set(libwebm_common_sources "${LIBWEBM_SRC_DIR}/common/file_util.cc"
                           "${LIBWEBM_SRC_DIR}/common/hdr_util.cc")

set(mkvmuxer_sources
    "${LIBWEBM_SRC_DIR}/mkvmuxer/mkvmuxer.cc"
    "${LIBWEBM_SRC_DIR}/mkvmuxer/mkvmuxerutil.cc"
    "${LIBWEBM_SRC_DIR}/mkvmuxer/mkvwriter.cc")

set(mkvmuxer_tests_sources "${LIBWEBM_TEST_DIR}/mkvmuxer_tests.cc"
                           "${LIBWEBM_TEST_DIR}/test_util.cc")

set(mkvparser_sources "${LIBWEBM_SRC_DIR}/mkvparser/mkvparser.cc"
                      "${LIBWEBM_SRC_DIR}/mkvparser/mkvreader.cc")

set(mkvparser_tests_sources "${LIBWEBM_TEST_DIR}/mkvparser_tests.cc"
                            "${LIBWEBM_TEST_DIR}/test_util.cc")

set(vp9_header_parser_tests_sources
    "${LIBWEBM_TEST_DIR}/common/vp9_header_parser_tests.cc"
    "${LIBWEBM_TEST_DIR}/common/vp9_header_parser.cc"
    "${LIBWEBM_TEST_DIR}/test_util.cc")

set(vp9_level_stats_tests_sources
    "${LIBWEBM_SRC_DIR}/common/vp9_header_parser.cc"
    "${LIBWEBM_TEST_DIR}/common/vp9_level_stats_tests.cc"
    "${LIBWEBM_SRC_DIR}/common/vp9_level_stats.cc"
    "${LIBWEBM_TEST_DIR}/test_util.cc")

set(vttdemux_sources "${LIBWEBM_SRC_DIR}/webm_parser/vttdemux.cc"
                     "${LIBWEBM_SRC_DIR}/webvtt/webvttparser.cc")

set(webm_parser_sources
    "${LIBWEBM_SRC_DIR}/webm_parser/ancestory.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/bit_utils.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/block_header_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/block_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/buffer_reader.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/callback.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/date_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/file_reader.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/float_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/id_element_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/id_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/istream_reader.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/master_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/parser_utils.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/segment_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/size_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/skip_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/unknown_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/var_int_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/virtual_block_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/void_parser.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/webm_parser.cc")

set(webm_parser_demo_sources
    "${CMAKE_CURRENT_LIST_DIR}/demos/webm_parser_demo.cc")
set(webm_parser_tests_sources
    "${LIBWEBM_TEST_DIR}/webm_parser/audio_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/bit_utils_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/block_additions_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/block_group_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/block_header_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/block_more_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/block_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/bool_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/buffer_reader_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/byte_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/callback_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/chapter_atom_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/chapter_display_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/chapters_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/cluster_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/colour_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/content_enc_aes_settings_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/content_encoding_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/content_encodings_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/content_encryption_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/cue_point_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/cue_track_positions_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/cues_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/date_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/ebml_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/edition_entry_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/element_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/float_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/id_element_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/id_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/info_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/int_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/istream_reader_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/limited_reader_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/master_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/master_value_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/mastering_metadata_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/parser_utils_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/projection_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/recursive_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/seek_head_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/seek_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/segment_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/simple_tag_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/size_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/skip_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/slices_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/tag_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/tags_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/targets_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/time_slice_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/track_entry_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/tracks_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/unknown_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/var_int_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/video_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/virtual_block_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/void_parser_test.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/webm_parser_test.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/test_utils/limited_reader.cc"
    "${LIBWEBM_TEST_DIR}/webm_parser/webm_parser_tests.cc")

set(webm_info_sources
    "${LIBWEBM_SRC_DIR}/common/indent.cc"
    "${LIBWEBM_SRC_DIR}/common/vp9_header_parser.cc"
    "${LIBWEBM_SRC_DIR}/common/vp9_level_stats.cc"
    "${LIBWEBM_SRC_DIR}/common/webm_endian.cc"
    "${LIBWEBM_SRC_DIR}/webm_parser/webm_info.cc")

set(webmts_sources
    "${LIBWEBM_SRC_DIR}/common/libwebm_util.cc"
    "${LIBWEBM_SRC_DIR}/common/video_frame.cc"
    "${LIBWEBM_SRC_DIR}/m2ts/vpxpes2ts.cc"
    "${LIBWEBM_SRC_DIR}/m2ts/vpxpes_parser.cc"
    "${LIBWEBM_SRC_DIR}/m2ts/webm2pes.cc")

set(webm2pes_sources "${LIBWEBM_SRC_DIR}/m2ts/webm2pes_main.cc")
set(webm2pes_tests_sources
    "${LIBWEBM_TEST_DIR}/test_util.cc"
    "${LIBWEBM_TEST_DIR}/video_frame_tests.cc"
    "${LIBWEBM_TEST_DIR}/m2ts/webm2pes_tests.cc")
set(webm2ts_sources "${LIBWEBM_SRC_DIR}/m2ts/vpxpes2ts_main.cc")

set(webvtt_common_sources "${LIBWEBM_SRC_DIR}/webvtt/vttreader.cc"
                          "${LIBWEBM_SRC_DIR}/webvtt/webvttparser.cc")

set(mkvmuxer_sample_sources
    "${CMAKE_CURRENT_LIST_DIR}/samples/mkvmuxer_sample.cc"
    "${CMAKE_CURRENT_LIST_DIR}/samples/sample_muxer_metadata.cc")

set(mkvparser_sample_sources
    "${CMAKE_CURRENT_LIST_DIR}/samples/mkvparser_sample.cc")

# Targets.
add_library(
  webm ${libwebm_common_sources} ${mkvmuxer_sources} ${mkvparser_sources}
       ${webvtt_common_sources} ${webm_parser_sources})
target_include_directories(
  webm
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/webm_parser/include)

add_executable(mkvparser_sample ${mkvparser_sample_sources})
target_link_libraries(mkvparser_sample LINK_PUBLIC webm)

add_executable(mkvmuxer_sample ${mkvmuxer_sample_sources})
target_link_libraries(mkvmuxer_sample LINK_PUBLIC webm)

add_executable(dumpvtt ${dumpvtt_sources})
target_link_libraries(dumpvtt LINK_PUBLIC webm)

add_executable(vttdemux ${vttdemux_sources})
target_link_libraries(vttdemux LINK_PUBLIC webm)

if(WEBM_ENABLE_WEBMINFO)
  add_executable(webm_info ${webm_info_sources})
  target_link_libraries(webm_info LINK_PUBLIC webm)
endif()

add_executable(webm_parser_demo ${webm_parser_demo_sources})
target_link_libraries(webm_parser_demo LINK_PUBLIC webm)

install(
  TARGETS webm
  EXPORT webm
  LIBRARY DESTINATION lib)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h*")

if(WEBM_ENABLE_WEBMTS)
  add_library(webmts OBJECT ${webmts_sources})
  target_link_libraries(webmts LINK_PUBLIC webm)

  add_executable(webm2pes ${webm2pes_sources} $<TARGET_OBJECTS:webmts>)
  target_link_libraries(webm2pes LINK_PUBLIC webm)

  add_executable(webm2ts ${webm2ts_sources} $<TARGET_OBJECTS:webmts>)
  target_link_libraries(webm2ts LINK_PUBLIC webm)
endif()

if(WEBM_ENABLE_TESTS)
  set(GTEST_SRC_DIR
      "${LIBWEBM_SRC_DIR}/../googletest"
      CACHE PATH "Path to Googletest git repository.")
  # This directory is where libwebm will build googletest dependencies.
  set(GTEST_BUILD_DIR "${CMAKE_BINARY_DIR}/googletest_build")

  if(LIBWEBM_DISABLE_GTEST_CMAKE)
    add_library(gtest STATIC "${GTEST_SRC_DIR}/googletest/src/gtest-all.cc")
    include_directories("${GTEST_SRC_DIR}/googletest")
  else()
    add_subdirectory("${GTEST_SRC_DIR}" "${GTEST_BUILD_DIR}")
  endif()
  include_directories("${GTEST_SRC_DIR}/googletest/include")

  add_executable(mkvmuxer_tests ${mkvmuxer_tests_sources})
  target_link_libraries(mkvmuxer_tests LINK_PUBLIC gtest webm)
  target_include_directories(mkvmuxer_tests PUBLIC testing)

  add_executable(mkvparser_tests ${mkvparser_tests_sources})
  target_link_libraries(mkvparser_tests LINK_PUBLIC gtest webm)
  target_include_directories(mkvparser_tests PUBLIC testing)

  add_executable(vp9_header_parser_tests ${vp9_header_parser_tests_sources})
  target_link_libraries(vp9_header_parser_tests LINK_PUBLIC gtest webm)
  target_include_directories(vp9_header_parser_tests PUBLIC testing)

  add_executable(vp9_level_stats_tests ${vp9_level_stats_tests_sources})
  target_link_libraries(vp9_level_stats_tests LINK_PUBLIC gtest webm)
  target_include_directories(vp9_level_stats_tests PUBLIC testing)

  if(WEBM_ENABLE_WEBMTS)
    add_executable(webm2pes_tests ${webm2pes_tests_sources}
                                  $<TARGET_OBJECTS:webmts>)
    target_link_libraries(webm2pes_tests LINK_PUBLIC gtest webm)
  endif()

  if(ENABLE_WEBM_PARSER)
    include_directories("${GTEST_SRC_DIR}/googlemock/include")
    add_executable(webm_parser_tests ${webm_parser_tests_sources})
    target_link_libraries(webm_parser_tests LINK_PUBLIC gmock gtest webm)
  endif()
endif()
