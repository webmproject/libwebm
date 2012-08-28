// Copyright (c) 2012 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#ifndef MKVSTREAM_H_
#define MKVSTREAM_H_

#include <cstdio>
#include <deque>
#include "./mkvparser.hpp"

class MkvStream : public mkvparser::IMkvReader {
 public:
  MkvStream();
  virtual ~MkvStream();

  // Open the file identified by |filename| in read-only mode, as a
  // binary stream of bytes.  Returns 0 on success, negative if error.
  int Open(const char* filename);

  // Closes the file stream.  Note that the stream is automatically
  // closed when the MkvStream object is destroyed.
  void Close();

  // Fetches |len| bytes of data from the cache, started at absolute
  // stream position |pos|, reading into buffer |buf|.  Returns
  // negative value if error (including mkvparser::E_BUFFER_NOT_FULL,
  // to indicate that not all of the requested bytes were in the
  // cache), 0 on success (all requested bytes were returned).
  virtual int Read(long long pos, long len, unsigned char* buf);

  // The |total| argument indicates how many total bytes are in the
  // stream.  This network simulator sets |total| to -1 until we reach
  // end-of-stream, at which point |total| is set to the file size.
  // The |available| argument indicates how much of the stream has been
  // consumed.  Returns negative on error, 0 on success.
  virtual int Length(long long* total, long long* available);

  // Read |len| bytes from the file stream into the cache, starting
  // at absolute file position |pos|.  This is a network simulator
  // so the actual number of bytes read into the cache might be less
  // than requested.  Returns negative if error, 0 on success.
  int PopulateCache(long long pos, long len);

  // Notify this reader that the stream up to (but not including)
  // offset |pos| has been parsed and is no longer of interest,
  // hence that portion of the stream can be removed from the cache.
  // Returns negative if error, 0 on success.
  int PurgeCache(long long pos);

 private:
  MkvStream(const MkvStream&);
  MkvStream& operator=(const MkvStream&);

  struct Page {
    int Read(MkvStream*);
    
    enum { kSize = 1024 };
    unsigned char buf_[kSize];
    long long off_;

    struct Less {
      bool operator()(const Page& page, long long pos) const {
        return (page.off_ < pos);
      }

      bool operator()(long long pos, const Page& page) const {
        return (pos < page.off_);
      }

      bool operator()(const Page& lhs, const Page& rhs) const {
        return (lhs.off_ < rhs.off_);
      }
    };
  };

  FILE* file_;

  typedef std::deque<Page> cache_t;
  cache_t cache_;

  long long total_;
  long long avail_;
};

#endif
