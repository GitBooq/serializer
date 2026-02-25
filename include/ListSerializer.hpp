// ListSerializer.hpp
#ifndef LIST_SERIALIZER_HPP
#define LIST_SERIALIZER_HPP

#include <concepts>
#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>

#include "List.hpp"

// Binary format:
//  NodesCount(32bit), [dataLen(32bit), data(dataLen bytes), randIdx(32bit)] *
//  NodesCount times

class ListSerializer {
private:
  const LinkedList &list_;
  std::unordered_map<const ListNode *, uint32_t> nodeToIdx_;

  static constexpr uint32_t NULL_INDEX{0xFFFFFFFF}; // -1
  static constexpr size_t DATA_MAX_SZ = 1000;

private:
  uint32_t getNodeCount() const {
    return static_cast<uint32_t>(nodeToIdx_.size());
  }

  static LinkedList buildList(const std::vector<std::string> &strings,
                              const std::vector<uint32_t> &rand_indices);

public:
  explicit ListSerializer(const LinkedList &list);

  // no copy
  ListSerializer(const ListSerializer &) = delete;
  ListSerializer &operator=(const ListSerializer &) = delete;

  // TODO move sema

  ~ListSerializer() = default;

  // Write
  bool toBinaryFile(const std::string &outFilename) const;

  // Read
  static LinkedList fromBinaryFile(const std::string &inputFilename);
};

#endif // LIST_SERIALIZER_HPP