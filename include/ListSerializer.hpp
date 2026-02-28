// ListSerializer.hpp
#ifndef LIST_SERIALIZER_HPP
#define LIST_SERIALIZER_HPP

#include <cstddef>       // for size_t
#include <cstdint>       // for uint32_t
#include <string>        // for string
#include <unordered_map> // for unordered_map
#include <vector>        // for vector
class LinkedList;
struct ListNode;

// Binary format:
//  NodesCount(32bit), [dataLen(32bit), data(dataLen bytes), randIdx(32bit)] *
//  NodesCount times

class ListSerializer {
private:
  const LinkedList *list_;
  std::unordered_map<const ListNode *, uint32_t>
      nodeToIdx_; // only for profiling.
                  // Better to use vector here

  static constexpr uint32_t NULL_INDEX{0xFFFFFFFF}; // -1
  static constexpr size_t DATA_MAX_SZ = 1000;

private:
  uint32_t getNodeCount() const {
    return static_cast<uint32_t>(nodeToIdx_.size());
  }

  static LinkedList buildList(const std::vector<std::string> &data,
                              const std::vector<uint32_t> &randIndices);

public:
  explicit ListSerializer(const LinkedList *list);

  // no copy
  ListSerializer(const ListSerializer &) = delete;
  ListSerializer &operator=(const ListSerializer &) = delete;

  // move
  ListSerializer(ListSerializer &&other) noexcept;
  ListSerializer &operator=(ListSerializer &&other) noexcept;

  ~ListSerializer() = default;

  // Write
  bool toBinaryFile(const std::string &outFilename) const;

  // Read
  static LinkedList fromBinaryFile(const std::string &inputFilename);
};

#endif // LIST_SERIALIZER_HPP
