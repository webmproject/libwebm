// Copyright (c) 2012 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#include "./mkvstream.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>

MkvStream::MkvStream() : file_(NULL) {
}

MkvStream::~MkvStream() {
  Close();
}

int MkvStream::Open(const char* filename) {
  if (filename == NULL || file_ != NULL)
    return -1;

  file_ = fopen(filename, "rb");
  if (file_ == NULL)
    return -1;

  total_ = -1;  // means "end-of-file not reached yet"
  avail_ = 0;

  // Establish invariant

  cache_.push_back(Page());
  Page& page = cache_.back();
  page.off_ = 0;

  const int e = page.Read(this);

  if (e < 0) {
    Close();
    return -1;
  }
    
  return 0;
}

void MkvStream::Close() {
  if (file_) {
    fclose(file_);
    file_ = NULL;

    cache_.clear();
  }
}

int MkvStream::Read(long long pos, long len, unsigned char* buf) {
  if (file_ == NULL)
    return -1;

  assert(!cache_.empty());  // invariant
  assert(avail_ >= cache_.back().off_);

  // For now, we only support sequential reading of streams, with no
  // jumps backwards in the stream.  Were this a real network cache,
  // we would have to purge the cache then reissue another fetch over
  // the wire.  (To be precise, this operation would report a cache
  // miss to the parser, which would be reported back to the caller as
  // an underflow.  The caller would then call PopulateCache directly,
  // then re-try the parse.)

  if (pos < 0)
    return -1;  // bad arg

  if (pos < cache_.front().off_)
    return -1;  // attempt to read non-sequentially

  if (total_ >= 0 && pos > total_)
    return -1;  // attempt to read beyond end-of-stream

  if (len < 0)
    return -1;  // bad arg

  if (len == 0)
    return 0;

  const long long end = pos + len;

  if (total_ >= 0 && end > total_)
    return -1;  // attempt to read beyond end of stream

  // Over a wire, network reads are slow, or block completely, which
  // is not acceptable (e.g. it would leave you unable to pump windows
  // messages).  Hence the need for a cache.  If we won't have enough
  // data in the cache to service this read call, we report underflow
  // to the parser, which in turn reports it back to the caller.  The
  // caller is then expected to populate the cache, using whatever
  // mechanism is appropriate for the application (e.g. post an async
  // read and wait for completion), and then re-try the parse.  This
  // is a simulator, so all the caller needs to do here is call
  // PopulateCache, but in a real application with real network I/O,
  // populating the cache can get very complex (especially when
  // seeking is supported).
  
  if (end > avail_)  // not enough data in the cache
    return mkvparser::E_BUFFER_NOT_FULL;

  if (buf == NULL)
    return -1;

  typedef cache_t::const_iterator iter_t;

  const iter_t i = cache_.begin();
  const iter_t j = cache_.end();

  const iter_t kk = std::upper_bound(i, j, pos, Page::Less());
  iter_t k = --iter_t(kk);

  while (len > 0) {
    assert(pos + len == end);

    const long long page_off = k->off_;
    assert(page_off <= pos);

    long long page_end = page_off + Page::kSize;
    if (page_end > end)
      page_end = end;

    const unsigned char* const src = k->buf_ + (pos - page_off);

    const long long count_ = page_end - pos;
    assert(count_ <= len);

    const size_t count = static_cast<size_t>(count_);
    memcpy(buf, src, count);

    pos += count;
    len -= count;
    buf += count;
  }

  assert(pos == end);
    
  return 0;
}

int MkvStream::Length(long long* total, long long* avail) {
  if (file_ == NULL || total == NULL || avail == NULL)
    return -1;

  *total = total_;
  *avail = avail_;

  return 0;
}

int MkvStream::PopulateCache(long long pos, long requested_len) {
  if (file_ == NULL)
    return -1;

  assert(!cache_.empty());
  assert(avail_ >= 0);
  assert(total_ < 0 || total_ == avail_);

  if (pos < 0)
    return -1;

  if (pos < cache_.front().off_)
    return -1;  // attempt to read non-sequentially

  if (requested_len < 0)
    return -1;

  if (requested_len == 0)
    return 0;  //TODO(matthewjheaney): ensure pos in cache?

  // Simulate a network read, which might not return all
  // requested bytes immediately:

  const long actual_len = 1 + rand() % requested_len;
  const long long end = pos + actual_len;

  long long off = cache_.back().off_;
  assert(off % Page::kSize == 0);
  assert(off <= avail_);

  while (total_ < 0 && avail_ < end) {
    cache_.push_back(Page());
    Page& page = cache_.back();

    off += Page::kSize;
    page.off_ = off;

    const int e = page.Read(this);

    if (e < 0)  // error
      return -1;

    assert(e == 0 || total_ >= 0);
  }

  return 0;
}
  
int MkvStream::PurgeCache(long long pos) {
  if (file_ == NULL)
    return -1;
  
  if (pos < 0)
    return -1;

  assert(!cache_.empty());

  if (pos < cache_.front().off_)
    return 0;

  typedef cache_t::iterator iter_t;

  iter_t i = cache_.begin();
  const iter_t j = cache_.end();
  const iter_t kk = std::upper_bound(i, j, pos, Page::Less());
  const iter_t k = --iter_t(kk);

  while (i != k)
    cache_.erase(i++);

  return 0;
}

int MkvStream::Page::Read(MkvStream* stream) {
  assert(stream);
  assert(stream->total_ < 0);
  assert(stream->avail_ >= 0);
  assert(off_ % kSize == 0);
  assert(off_ == stream->avail_);

  FILE* const f = stream->file_;
  assert(f);

  unsigned char* dst = buf_;

  for (int i = 0; i < kSize; ++i) {
    const int c = fgetc(f);

    if (c == EOF) {
      if (!feof(f))
        return -1;

      stream->total_ = stream->avail_;
      return 1;
    }

    *dst++ = static_cast<unsigned char>(c);
    ++stream->avail_;
  }

  return 0;
}  
