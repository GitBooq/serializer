// main.cpp
// A doubly linked list serialization with random references
// Input file: inlet.in
// Format:     <data>;<rand_index>
//
//             -<data> -- string (ws, special chars, UTF-8)
//             -<rand_index> -- index of node, which rand points to or -1 if
//             nullptr
//
// Output: binary file outlet.out containing serialized doubly linked list
//  Binary Representation: NodesCount(32bit), [dataLen(32bit), data(dataLen
//  bytes), randIdx(32bit)] * NodesCount times
//
// Constraints: max nodes count 10^6
//              0 <= data.length() <= 1000
// Example:
//  input file: inlet.in:
//          apple;2
//          banana;-1
//          carrot;1
//  list:
//          node 0: "apple"  → rand to node 2
//          node 1: "banana" → rand = nullptr
//          node 2: "carrot" → rand to node 1
//  output file: outlet.out
//

#include <cstdlib>             // for EXIT_FAILURE
#include <iostream>            // for char_traits, basic_ostream, endl, oper...
#include <string>              // for allocator, string, operator<<
#include "List.hpp"            // for LinkedList, operator==, ListBuilder
#include "ListSerializer.hpp"  // for ListSerializer

int main() {
  static std::string inputFilename{"inlet.in"};
  static std::string outputFilename{"outlet.out"};

  // Read file and build list
  LinkedList list = ListBuilder::fromTextFile(inputFilename);

  // Serialization
  ListSerializer ls(&list);
  if (!ls.toBinaryFile(outputFilename)) {
    std::cerr << "Can't open file" << std::endl;
    return EXIT_FAILURE;
  }

  LinkedList restoredList = ListSerializer::fromBinaryFile(outputFilename);

  std::string testMsg;
  testMsg = (list == restoredList) ? "passed" : "failed";
  std::cout << testMsg << std::endl;
}
