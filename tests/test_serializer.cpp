#include <gtest/gtest.h>
#include <string_view>
#include <fstream>

#include "List.hpp"
#include "ListSerializer.hpp"

struct ListSerializerTest
    : public ::testing::Test
{
    LinkedList list = ListBuilder::fromTextFile(TEST_DATA_DIR "/inlet.in");
    ListSerializer ls{&list};

    virtual void SetUp() override {}

    virtual void TearDown() override {}
};

TEST_F(ListSerializerTest, CorrectRawData) {
    std::string outFile = "outlet.out";
    bool isOk = ls.toBinaryFile(outFile);
    EXPECT_EQ(true, isOk) << "Error openning file";

    std::ifstream file(outFile, std::ios::binary);
    std::stringstream buf;
    if (file) {
        buf << file.rdbuf();
        file.close();
    }
    std::string_view result = buf.view();

//apple;2
//banana;-1
//carrot;1

    std::string nodesCnt_Raw("\x03\x00\x00\x00", 4);

    std::string data_1_Size_Raw("\x05\x00\x00\x00", 4);
    std::string data_1{"apple"};
    std::string data_1_index_Raw("\x02\x00\x00\x00", 4);

    std::string data_2_Size_Raw("\x06\x00\x00\x00", 4);
    std::string data_2{"banana"};
    std::string data_2_index_Raw("\xFF\xFF\xFF\xFF", 4);

    std::string data_3_Size_Raw("\x06\x00\x00\x00", 4);
    std::string data_3{"carrot"};
    std::string data_3_index_Raw("\x01\x00\x00\x00", 4);
    
    std::string etalon {nodesCnt_Raw 
        + data_1_Size_Raw + data_1 + data_1_index_Raw
        + data_2_Size_Raw + data_2 + data_2_index_Raw
        + data_3_Size_Raw + data_3 + data_3_index_Raw
    };

    EXPECT_EQ(etalon.size(), result.size()) << "Size mismatch";
    EXPECT_EQ(etalon, result) << "Data mismatch";
}

