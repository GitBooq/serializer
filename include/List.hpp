// List.hpp
#ifndef LIST_HPP
#define LIST_HPP

#include <cstdint>        // for uint32_t
#include <cstddef>        // for size_t, ptrdiff_t
#include <iterator>       // for bidirectional_iterator_tag
#include <memory>         // for unique_ptr
#include <string>         // for string
#include <unordered_map>  // for unordered_map
#include <vector>         // for vector

struct ListNode {
  ListNode *prev = nullptr; // указатель на предыдущий элемент или nullptr
  ListNode *next = nullptr;
  ListNode *rand = nullptr; // указатель на произвольный элемент данного
                            // списка, либо `nullptr`
  std::string data;         // произвольные пользовательские данные
};

// RAII Class list owner
//
class LinkedList {
private:
  // custom deleter for unique_ptr
  //
  struct Deleter {
    void operator()(ListNode *head) const;
  };

  std::unique_ptr<ListNode, Deleter> head_; // list head
  size_t size_ = 0;                         // number of nodes

private:
  friend class ListBuilder; // create list from here

  // replace list by new one
  //
  void setHead(ListNode *head) { head_.reset(head); }
  // size setter
  //
  void setSize(size_t size) { size_ = size; }
  // return ptr to head node and release ownership
  //
  ListNode *releaseHead() { return head_.release(); }

public:
  LinkedList() = default;

  // no copy
  LinkedList(const LinkedList &) = delete;
  LinkedList &operator=(const LinkedList &) = delete;

  // move
  LinkedList(LinkedList &&) = default;
  LinkedList &operator=(LinkedList &&) = default;

  ~LinkedList() = default;

  class iterator final {
  private:
    using T = ListNode;
    friend class LinkedList;
    T *ptr_ = nullptr;

    iterator(T *ptr) noexcept : ptr_(ptr) {}

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using pointer = T *;

    iterator() = default;
    iterator(const iterator &) = default;
    iterator &operator=(const iterator &) = default;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    iterator &operator++() noexcept;
    iterator operator++(int) noexcept;
    iterator &operator--() noexcept;
    iterator operator--(int) noexcept;

    auto operator<=>(const iterator &) const noexcept = default;
  };

  class const_iterator final {
  private:
    using T = ListNode;
    friend class LinkedList;
    const T *ptr_ = nullptr;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using reference = const T &;
    using pointer = const T *;

    const_iterator() = default;
    explicit const_iterator(const T *ptr) noexcept : ptr_(ptr) {}
    const_iterator(const const_iterator &) = default;
    const_iterator &operator=(const const_iterator &) = default;

    const T &operator*() const noexcept;
    pointer operator->() const noexcept;
    const_iterator &operator++() noexcept;
    const_iterator operator++(int) noexcept;
    const_iterator &operator--() noexcept;
    const_iterator operator--(int) noexcept;

    auto operator<=>(const const_iterator &) const noexcept = default;
  };

  iterator begin() { return iterator(head_.get()); }
  iterator end() { return iterator(nullptr); }
  const_iterator begin() const noexcept { return const_iterator(head_.get()); }
  const_iterator end() const noexcept { return const_iterator(nullptr); }
  const_iterator cbegin() const noexcept { return const_iterator(head_.get()); }
  const_iterator cend() const noexcept { return const_iterator(nullptr); }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  friend bool operator==(const LinkedList &lhs, const LinkedList &rhs);
};

// Builder for list
class ListBuilder {
public:
  // build LinkedList from text file <filename>
  static LinkedList fromTextFile(const std::string &filename);

  // build LinkedList from vector of strings and vector of random indexes
  static LinkedList fromMemory(const std::vector<std::string> &data,
                               const std::vector<uint32_t> &randIndices);
};

std::unordered_map<const ListNode *, uint32_t>
buildIndexMap(const LinkedList &list);
#endif // LIST_HPP
