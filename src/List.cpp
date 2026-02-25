// List.cpp
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "List.hpp"

void LinkedList::Deleter::operator()(ListNode *head) const {
  while (head) {
    ListNode *next = head->next;
    delete head;
    head = next;
  }
}

LinkedList ListBuilder::fromTextFile(const std::string &filename) {
  LinkedList list;

  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cerr << "Can't open file " << filename << '\n';
    return {};
  }

  std::vector<std::unique_ptr<ListNode>> nodes;
  std::vector<int> randIndices;

  const char separator = ';';
  std::string line;

  // Read file and create nodes
  while (std::getline(in, line)) {
    size_t semicolonPos = line.find(separator);
    if (semicolonPos == std::string::npos) {
      std::cerr << "incorrect data format\n";
      return {};
    }

    std::string data = line.substr(0, semicolonPos);
    int randIndex = std::stoi(line.substr(semicolonPos + 1));

    auto node = std::make_unique<ListNode>();
    node->data = data;

    nodes.push_back(std::move(node));
    randIndices.push_back(randIndex);
  }

  if (nodes.empty())
    return list;

  // Linking prev & next
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (i > 0)
      nodes[i]->prev = nodes[i - 1].get();
    if (i < nodes.size() - 1)
      nodes[i]->next = nodes[i + 1].get();
  }

  // Linking rand
  for (size_t i = 0; i < nodes.size(); i++) {
    if (randIndices[i] >= 0 &&
        randIndices[i] < static_cast<int>(nodes.size())) {
      nodes[i]->rand = nodes[randIndices[i]].get();
    }
  }

  // Move ownership of first node to List
  list.setHead(nodes[0].release());
  list.setSize(nodes.size());
  // release other nodes
  for (size_t i = 1; i < nodes.size(); ++i) {
    nodes[i].release();
  }

  return list;
}

LinkedList ListBuilder::fromMemory(const std::vector<std::string> &vdata,
                                   const std::vector<uint32_t> &randIndices) {
  LinkedList list;

  std::vector<std::unique_ptr<ListNode>> nodes;

  for (const auto &data : vdata) {
    auto node = std::make_unique<ListNode>();
    node->data = data;
    nodes.push_back(std::move(node));
  }

  if (nodes.empty())
    return list;

  // link prev/next
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (i > 0)
      nodes[i]->prev = nodes[i - 1].get();
    if (i < nodes.size() - 1)
      nodes[i]->next = nodes[i + 1].get();
  }

  // link rand
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (randIndices[i] >= 0 &&
        randIndices[i] < static_cast<uint32_t>(nodes.size())) {
      nodes[i]->rand = nodes[randIndices[i]].get();
    }
  }

  // Move ownership of first node to List
  list.setHead(nodes[0].release());
  list.setSize(nodes.size());
  // release other nodes
  for (size_t i = 1; i < nodes.size(); ++i) {
    nodes[i].release();
  }

  return list;
}

std::unordered_map<const ListNode *, uint32_t>
buildIndexMap(const LinkedList &list) {
  std::unordered_map<const ListNode *, uint32_t> indexes;

  uint32_t idx = 0;
  for (const auto &node : list) {
    indexes[&node] = idx++;
  }

  return indexes;
}

bool operator==(const LinkedList &lhs, const LinkedList &rhs) {
  if (lhs.size() != rhs.size())
    return false;

  std::unordered_map<const ListNode *, uint32_t> index_lhs = buildIndexMap(lhs);
  std::unordered_map<const ListNode *, uint32_t> index_rhs = buildIndexMap(rhs);

  auto lhsIt = lhs.begin();
  auto rhsIt = rhs.begin();

  for (; lhsIt != lhs.end(); ++lhsIt, ++rhsIt) {
    if (lhsIt->data != rhsIt->data)
      return false;

    if (lhsIt->rand == nullptr && rhsIt->rand == nullptr)
      continue;
    if (lhsIt->rand == nullptr || rhsIt->rand == nullptr)
      return false;

    size_t idx_lhs = index_lhs[lhsIt->rand];
    size_t idx_rhs = index_rhs[rhsIt->rand];
    if (idx_lhs != idx_rhs)
      return false;
  }

  return true;
}

// Iterators
using LLIter = LinkedList::iterator;
LLIter::reference LLIter::operator*() const noexcept { return *ptr_; }
LLIter::pointer LLIter::operator->() const noexcept { return ptr_; }
LLIter &LLIter::operator++() noexcept {
  ptr_ = ptr_->next;
  return *this;
}
LLIter LLIter::operator++(int) noexcept {
  iterator copy{*this};
  ptr_ = ptr_->next;
  return copy;
}
LLIter &LLIter::operator--() noexcept {
  ptr_ = ptr_->prev;
  return *this;
}
LLIter LLIter::operator--(int) noexcept {
  iterator copy{*this};
  ptr_ = ptr_->prev;
  return copy;
}

using LLCIter = LinkedList::const_iterator;
const LLCIter::T &LLCIter::operator*() const noexcept { return *ptr_; }
LLCIter::pointer LLCIter::operator->() const noexcept { return ptr_; }
LLCIter &LLCIter::operator++() noexcept {
  ptr_ = ptr_->next;
  return *this;
}
LLCIter LLCIter::operator++(int) noexcept {
  const_iterator tmp{*this};
  ptr_ = ptr_->next;
  return tmp;
}
LLCIter &LLCIter::operator--() noexcept {
  ptr_ = ptr_->prev;
  return *this;
}
LLCIter LLCIter::operator--(int) noexcept {
  const_iterator tmp{*this};
  ptr_ = ptr_->prev;
  return tmp;
}