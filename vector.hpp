#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <memory>
#include <stdexcept>

namespace custom {
template <typename ValueType> class DynamicArray {
private:
  ValueType *storage_ = nullptr;
  std::size_t count_ = 0;
  std::size_t reserved_ = 0;

  void expandStorageIfNeeded() {
    if (count_ >= reserved_) {
      std::size_t new_reserved = (reserved_ == 0) ? 2 : (reserved_ * 2);
      reallocateStorage(new_reserved);
    }
  }

  void reallocateStorage(std::size_t new_reserved) {
    ValueType *new_storage = new ValueType[new_reserved];

    for (std::size_t i = 0; i < count_; ++i) {
      new_storage[i] = std::move(storage_[i]);
    }

    delete[] storage_;
    storage_ = new_storage;
    reserved_ = new_reserved;
  }

  void validateIndex(std::size_t position) const {
    if (position >= count_) {
      throw std::out_of_range("position out of bounds");
    }
  }

public:
  DynamicArray() = default;

  explicit DynamicArray(std::size_t initial_count)
      : storage_(new ValueType[initial_count]{}), count_(initial_count),
        reserved_(initial_count) {}

  DynamicArray(std::size_t initial_count, const ValueType &default_value)
      : storage_(new ValueType[initial_count]), count_(initial_count),
        reserved_(initial_count) {
    for (std::size_t i = 0; i < count_; ++i) {
      storage_[i] = default_value;
    }
  }

  DynamicArray(std::initializer_list<ValueType> init_list)
      : storage_(new ValueType[init_list.size()]), count_(init_list.size()),
        reserved_(init_list.size()) {
    std::size_t index = 0;
    for (const auto &item : init_list) {
      storage_[index++] = item;
    }
  }

  ~DynamicArray() { clearMemory(); }

  DynamicArray(const DynamicArray &other)
      : storage_(new ValueType[other.reserved_]), count_(other.count_),
        reserved_(other.reserved_) {
    for (std::size_t i = 0; i < count_; ++i) {
      storage_[i] = other.storage_[i];
    }
  }

  DynamicArray(DynamicArray &&other) noexcept
      : storage_(other.storage_), count_(other.count_),
        reserved_(other.reserved_) {
    other.storage_ = nullptr;
    other.count_ = other.reserved_ = 0;
  }

  DynamicArray &operator=(const DynamicArray &other) {
    if (this != &other) {
      clearMemory();
      count_ = other.count_;
      reserved_ = other.reserved_;
      storage_ = new ValueType[reserved_];
      for (std::size_t i = 0; i < count_; ++i) {
        storage_[i] = other.storage_[i];
      }
    }
    return *this;
  }

  DynamicArray &operator=(DynamicArray &&other) noexcept {
    if (this != &other) {
      clearMemory();
      storage_ = other.storage_;
      count_ = other.count_;
      reserved_ = other.reserved_;
      other.storage_ = nullptr;
      other.count_ = other.reserved_ = 0;
    }
    return *this;
  }

  void append(const ValueType &value) {
    expandStorageIfNeeded();
    storage_[count_++] = value;
  }

  void append(ValueType &&value) {
    expandStorageIfNeeded();
    storage_[count_++] = std::move(value);
  }

  void removeLast() {
    if (count_ > 0) {
      --count_;
    }
  }

  ValueType &getElementAt(std::size_t position) {
    validateIndex(position);
    return storage_[position];
  }

  const ValueType &getElementAt(std::size_t position) const {
    validateIndex(position);
    return storage_[position];
  }

  ValueType &directAccess(std::size_t position) { return storage_[position]; }

  const ValueType &directAccess(std::size_t position) const {
    return storage_[position];
  }

  std::size_t currentSize() const noexcept { return count_; }
  std::size_t totalCapacity() const noexcept { return reserved_; }
  bool isEmpty() const noexcept { return count_ == 0; }

  void ensureCapacity(std::size_t new_capacity) {
    if (new_capacity > reserved_) {
      reallocateStorage(new_capacity);
    }
  }

  void shrinkToFit() {
    if (reserved_ > count_) {
      reallocateStorage(count_);
    }
  }

  void clear() { count_ = 0; }

  ValueType *begin() noexcept { return storage_; }
  const ValueType *begin() const noexcept { return storage_; }
  ValueType *end() noexcept { return storage_ + count_; }
  const ValueType *end() const noexcept { return storage_ + count_; }

private:
  void clearMemory() {
    delete[] storage_;
    storage_ = nullptr;
    count_ = reserved_ = 0;
  }
};
} // namespace custom

#endif
