/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#ifndef ARGCV_CXX_DB_LEVELDB_HELPER_H_
#define ARGCV_CXX_DB_LEVELDB_HELPER_H_

#include <cstdio>

#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/write_batch.h"

namespace argcv {

typedef std::function<bool(const std::string&, const std::string&)>
    LdbKvHandler;

static inline ::leveldb::Options LdbDefaultOpt() {
  ::leveldb::Options opt;
  return opt;
}

static inline ::leveldb::DB* LdbInit(const std::string& ddir,
                                     ::leveldb::Options* opt,
                                     size_t cache_size = 0L,
                                     bool create_if_missing = true) {
  if (opt == nullptr) {
    return nullptr;
  }
  opt->create_if_missing = create_if_missing;
  if (cache_size > 0) {
    opt->block_cache = ::leveldb::NewLRUCache(cache_size);
  }
  ::leveldb::DB* db;
  return ::leveldb::DB::Open(*opt, ddir, &db).ok() ? db : nullptr;
}

static inline bool LdbClose(::leveldb::DB* db, ::leveldb::Options* opt) {
  if (db != nullptr) {
    delete db;
  }
  if (opt != nullptr && opt->block_cache != nullptr) {
    delete opt->block_cache;
  }
  return true;
}

static inline bool LdbStartsWith(::leveldb::DB* db, const std::string& base,
                                 LdbKvHandler kvs) {
  ::leveldb::ReadOptions snap_read_opt;
  snap_read_opt.snapshot = db->GetSnapshot();
  ::leveldb::Iterator* it = db->NewIterator(snap_read_opt);
  ::leveldb::Slice slice = base;
  for (it->Seek(slice);
       it->Valid() &&
       std::mismatch(base.begin(), base.end(), it->key().ToString().begin())
               .first == base.end();
       it->Next()) {
    if (!kvs(it->key().ToString(), it->value().ToString())) {
      break;
    }
  }
  delete it;
  db->ReleaseSnapshot(snap_read_opt.snapshot);
  return true;
}

static inline bool LdbIter(::leveldb::DB* db, const std::string& base,
                           LdbKvHandler kvs) {
  ::leveldb::ReadOptions snap_read_opt;
  snap_read_opt.snapshot = db->GetSnapshot();
  ::leveldb::Iterator* it = db->NewIterator(snap_read_opt);
  ::leveldb::Slice slice = base;
  for (it->Seek(slice); it->Valid(); it->Next()) {
    if (!kvs(it->key().ToString(), it->value().ToString())) {
      break;
    }
  }
  delete it;
  db->ReleaseSnapshot(snap_read_opt.snapshot);
  return true;
}

static inline bool LdbExist(::leveldb::DB* db, const std::string& key) {
  std::string val;
  return ((db->Get(::leveldb::ReadOptions(), key, &val)).ok());
}

static inline bool LdbPut(::leveldb::DB* db, const std::string& key,
                          const std::string& val) {
  return ((db->Put(::leveldb::WriteOptions(), key, val)).ok());
}

static inline bool LdbGet(::leveldb::DB* db, const std::string& key,
                          std::string* _val) {
  return ((db->Get(::leveldb::ReadOptions(), key, _val)).ok());
}

static inline bool LdbDelete(::leveldb::DB* db, const std::string& key) {
  return ((db->Delete(::leveldb::WriteOptions(), key)).ok());
}

static inline bool LdbBatchPut(::leveldb::DB* db,
                               const std::map<std::string, std::string>& kvs) {
  ::leveldb::WriteBatch* wb = new ::leveldb::WriteBatch;
  for (std::map<std::string, std::string>::const_iterator it = kvs.begin();
       it != kvs.end(); it++) {
    wb->Put(it->first, it->second);
  }
  bool rst = ((db->Write(::leveldb::WriteOptions(), wb)).ok());
  delete wb;
  return rst;
}

static inline bool LdbBatchDelete(::leveldb::DB* db,
                                  const std::set<std::string>& keys) {
  ::leveldb::WriteBatch* wb = new ::leveldb::WriteBatch;
  for (std::set<std::string>::const_iterator it = keys.begin();
       it != keys.end(); it++) {
    wb->Delete(*it);
  }
  bool rst = ((db->Write(::leveldb::WriteOptions(), wb)).ok());
  delete wb;
  return rst;
}

static inline bool LdbDestroy(const std::string& ddir) {
  return ::leveldb::DestroyDB(ddir, ::leveldb::Options()).ok();
}

class LdbBatchOperator {
 public:
  explicit LdbBatchOperator(::leveldb::DB* db)
      : db_(db), wb_(new ::leveldb::WriteBatch) {}
  ~LdbBatchOperator() {
    db_->Write(::leveldb::WriteOptions(), wb_);
    delete wb_;
  }

  void Put(const std::string& key, const std::string& val) {
    wb_->Put(key, val);
  }

  void Delete(const std::string& key) { wb_->Delete(key); }

  bool Flush() {
    ::leveldb::WriteBatch* pwb = wb_;
    wb_ = new ::leveldb::WriteBatch;
    bool status = (db_->Write(::leveldb::WriteOptions(), pwb)).ok();
    delete pwb;
    return status;
  }

 private:
  ::leveldb::DB* db_;
  ::leveldb::WriteBatch* wb_;
};

}  // namespace argcv

#endif  // ARGCV_CXX_DB_LEVELDB_HELPER_H_
