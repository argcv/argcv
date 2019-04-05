// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_SIGNAL_SLOT_H_
#define ARGCV_CXX_BASE_SIGNAL_SLOT_H_

#include <cstdint>
#include <functional>
#include <map>

#include "argcv/cxx/base/macros.h"

namespace argcv {

template <typename... Args>
class SignalSlot {
 public:
  typedef std::function<void(Args...)> Functor;

  SignalSlot() noexcept : top_id_(0) {}

  virtual ~SignalSlot() noexcept {}

  uint64_t Connect(Functor&& func) noexcept {
    uint64_t id = top_id_++;
    functors_.emplace(std::make_pair(id, func));
    return id;
  }

  bool Disconnect(uint64_t id) noexcept {
    if (functors_.find(id) != functors_.end()) {
      functors_.erase(id);
      // found
      return true;
    } else {
      // not exists
      return false;
    }
  }

  bool DisconnectAll() noexcept { functors_.clear(); }

  void Call(Args&&... args) noexcept {
    for (auto it : functors_) {
      it.second(args...);
    }
  }

 private:
  std::map<uint64_t, Functor> functors_;
  uint64_t top_id_;
};
}  // namespace argcv

#endif  // ARGCV_CXX_BASE_SIGNAL_SLOT_H_
