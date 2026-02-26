// ListSerializer.cpp
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


#include "ListSerializer.hpp"

// HELPERS
namespace {
// Endianness convertors

// Convert integral types to Little-endian byte order
//
template <std::integral T> static T ToLittleEndian(T value) {
  if constexpr (std::endian::native == std::endian::big) {
    // reverse byte order
    T reversed = 0;
    for (size_t byteIndex = 0; byteIndex < sizeof(T); ++byteIndex) {
      const T byteMask = 0xFF;
      T sourceByte = ((value >> (byteIndex * CHAR_BIT)) & byteMask);
      size_t destOffset = (sizeof(T) - 1 - byteIndex) * CHAR_BIT;
      reversed |= sourceByte << destOffset;
    }
    return reversed;
  } else {
    return value; // no change required on Little-endian systems
  }
}

// Convert integral types to Little-endian byte order
//
template <std::integral T> static T FromLittleEndian(T value) {
  return ToLittleEndian(value);
}

// Write raw bytes from value to output stream (os) taking into account
// Endianess if value is integral type
//
template <typename T>
  requires std::is_trivially_copyable_v<T>
static void Write(std::ostream &os, const T &value) {
  if constexpr (std::is_integral_v<T>) {
    T le_value = ToLittleEndian(value);
    os.write(reinterpret_cast<const char *>(&le_value), sizeof(T));
  } else {
    os.write(reinterpret_cast<const char *>(&value), sizeof(T));
  }
}

// Overload for c-style strings
// Write len chars from data to output stream (os)
//
static void Write(std::ostream &os, const char *data, size_t len) {
  os.write(data, len);
}

// Read raw bytes from istream (is) to value taking into account Endianess
// if value is integral type
//
template <typename T>
  requires std::is_trivially_copyable_v<T>
static bool Read(std::istream &is, T &value) {
  is.read(reinterpret_cast<char *>(&value), sizeof(T));
  if (is.fail() || is.gcount() != sizeof(T)) {
    return false; // read error
  }
  if constexpr (std::is_integral_v<T>) {
    value = FromLittleEndian(value);
  }
  return true;
}

// Overload for c-style strings
// Read len chars from istream (is) to char buffer
//
static bool Read(std::istream &is, char *buf, size_t len) { 
  is.read(buf, len);
  if (is.fail() || static_cast<size_t>(is.gcount()) != len) {
    return false; // read error
  }
  return true;
}

} // namespace

ListSerializer::ListSerializer(const LinkedList *list) : list_(list) {
  auto indexMap = buildIndexMap(*list);
  nodeToIdx_ = std::move(indexMap);
}

ListSerializer::ListSerializer(ListSerializer &&other) noexcept
    : list_(std::exchange(other.list_, nullptr)),
      nodeToIdx_(std::move(other.nodeToIdx_)) {}

ListSerializer &ListSerializer::operator=(ListSerializer &&other) noexcept {
  if (this != &other) {
    list_ = std::exchange(other.list_, nullptr);
    nodeToIdx_ = std::move(other.nodeToIdx_);
  }
  return *this;
}

// Binary Representation:
//  NodesCount(32bit), [dataLen(32bit), data(dataLen bytes), randIdx(32bit)] *
//  NodesCount times
//
bool ListSerializer::toBinaryFile(const std::string &outFilename) const {
  std::ofstream out(outFilename, std::ios::binary);
  if (!out.is_open())
    return false;

  /*  write nodesCnt */
  uint32_t nodesCnt = getNodeCount();
  Write(out, nodesCnt);

  for (const auto &node : *list_) {
    /* write data length */
    uint32_t dataLen = static_cast<uint32_t>(node.data.length());
    Write(out, dataLen);

    /* write data */
    const char *data = node.data.c_str();
    Write(out, data, dataLen);

    /* write rand index */
    uint32_t randIdx =
        (node.rand) ? (nodeToIdx_.find(node.rand))->second : NULL_INDEX;
    Write(out, randIdx);
  }
  return true;
}

LinkedList ListSerializer::fromBinaryFile(const std::string &inputFilename) {
  std::ifstream input(inputFilename, std::ios::binary);
  if (!input.is_open()) {
    std::cerr << "Can't open file " << inputFilename << '\n';
    return {};
  }

  // read nodesCnt
  uint32_t nodesCnt;
  if (!Read(input, nodesCnt)) {
    std::cerr << "Read error\n";
    return {};
  }

  std::vector<std::string> data(nodesCnt);
  std::vector<uint32_t> randIndices(nodesCnt);

  for (uint32_t i = 0; i < nodesCnt; ++i) {
    // read data length
    uint32_t dataLen;
    if (!Read(input, dataLen)) {
      std::cerr << "Read error\n";
      return {};
    }
    if (dataLen > DATA_MAX_SZ) {
      std::cerr << "Invalid data size\n";
      return {};
    }

    // read data
    data[i].resize(dataLen);
    if (!Read(input, data[i].data(), static_cast<size_t>(dataLen))) {
      std::cerr << "Read error\n";
      return {};
    }

    // read rand index
    if (!Read(input, randIndices[i])) {
      std::cerr << "Read error\n";
      return {};
    }
  }

  return buildList(data, randIndices);
}

LinkedList
ListSerializer::buildList(const std::vector<std::string> &data,
                          const std::vector<uint32_t> &randIndices) {
  return ListBuilder::fromMemory(data, randIndices);
}
