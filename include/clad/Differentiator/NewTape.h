#ifndef CLAD_NEWTAPE_H
#define CLAD_NEWTAPE_H

#include <cassert>
#include <cstdio>
#include <memory>
#include <type_traits>
#include <utility>

namespace clad {

template <typename T> class Block {
public:
  T* _data;
  Block<T>* next;
  Block<T>* prev;

  Block(std::size_t& _capacity) {
    _data =
        static_cast<T*>(::operator new(_capacity * sizeof(T), std::nothrow));
    next = nullptr;
    prev = nullptr;
  }

  ~Block() {}
};

template <typename T> class new_tape_impl {
  Block<T>* cur_block = nullptr;
  std::size_t _capacity = 32;
  std::size_t _size = 0;
  using pointer = T*;
  using reference = T&;

  using iterator = pointer;

public:
  template <typename... ArgsT>

  void emplace_back(ArgsT&&... args) {
    if (!cur_block || _size >= _capacity) {
      Block<T>* prev_block = cur_block;
      cur_block = new Block<T>(_capacity);
      if (prev_block) {
        prev_block->next = cur_block;
        cur_block->prev = prev_block;
      }
      _size = 0;
    }
    _size += 1;
    ::new (const_cast<void*>(static_cast<const volatile void*>(block_end())))
        T(std::forward<ArgsT>(args)...);
  }

  std::size_t size() const { return _size; }

  iterator block_begin() {
    return reinterpret_cast<iterator>(cur_block->_data);
  }

  iterator block_end() {
    return reinterpret_cast<iterator>(cur_block->_data + _size - 1);
  }

  reference back() {
    assert(_size || cur_block->prev);
    return block_begin()[_size - 1];
  }

  void pop_back() {
    assert(_size || cur_block->prev);
    block_end()->~T();
    _size -= 1;
    if (_size == 0) {
      Block<T>* temp = cur_block;
      cur_block = cur_block->prev;
      temp->~Block<T>();
      _size = _capacity;
    }
  }
};
} // namespace clad

#endif // CLAD_NEWTAPE_H
