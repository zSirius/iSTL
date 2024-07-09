#include <gtest/gtest.h>
#include <vector>
#include "vector.h" 

// 测试默认构造函数
TEST(IstlVectorTest, DefaultConstructor) {
    istl::vector<int> myVector;
    std::vector<int> stdVector;

    EXPECT_EQ(myVector.size(), stdVector.size());
    EXPECT_TRUE(myVector.empty());
    EXPECT_TRUE(stdVector.empty());
}

// 测试大小构造函数
TEST(IstlVectorTest, SizeConstructor) {
    istl::vector<int> myVector(5);
    std::vector<int> stdVector(5);

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试大小和初始值构造函数
TEST(IstlVectorTest, SizeValueConstructor) {
    istl::vector<int> myVector(5, 10);
    std::vector<int> stdVector(5, 10);

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试push_back函数
TEST(IstlVectorTest, PushBack) {
    istl::vector<int> myVector;
    std::vector<int> stdVector;

    for (int i = 0; i < 10; ++i) {
        myVector.push_back(i);
        stdVector.push_back(i);
    }

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试pop_back函数
TEST(IstlVectorTest, PopBack) {
    istl::vector<int> myVector;
    std::vector<int> stdVector;

    for (int i = 0; i < 10; ++i) {
        myVector.push_back(i);
        stdVector.push_back(i);
    }

    for (int i = 0; i < 5; ++i) {
        myVector.pop_back();
        stdVector.pop_back();
    }

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试插入函数
TEST(IstlVectorTest, Insert) {
    istl::vector<int> myVector(5, 1);
    std::vector<int> stdVector(5, 1);

    myVector.insert(myVector.begin() + 2, 99);
    stdVector.insert(stdVector.begin() + 2, 99);

    myVector.insert(myVector.begin()+3, 20, 20);
    stdVector.insert(stdVector.begin()+3, 20, 20);

    myVector.insert(myVector.begin(), myVector.begin()+5, myVector.begin()+10);
    stdVector.insert(stdVector.begin(), stdVector.begin()+5, stdVector.begin()+10);



    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < myVector.size(); ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试擦除函数
TEST(IstlVectorTest, Erase) {
    istl::vector<int> myVector(50, 1);
    std::vector<int> stdVector(50, 1);

    myVector.erase(myVector.begin() + 2);
    stdVector.erase(stdVector.begin() + 2);

    myVector.erase(myVector.begin() + 10, myVector.begin()+20);
    stdVector.erase(stdVector.begin() + 10, stdVector.begin()+20);

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < myVector.size(); ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试清除函数
TEST(IstlVectorTest, Clear) {
    istl::vector<int> myVector(5, 1);
    std::vector<int> stdVector(5, 1);

    myVector.clear();
    stdVector.clear();

    EXPECT_EQ(myVector.size(), stdVector.size());
    EXPECT_TRUE(myVector.empty());
    EXPECT_TRUE(stdVector.empty());
}

// 测试reserve函数
TEST(IstlVectorTest, Reserve) {
    istl::vector<int> myVector;
    std::vector<int> stdVector;

    myVector.reserve(100);
    stdVector.reserve(100);

    EXPECT_EQ(myVector.capacity(), stdVector.capacity());

    for (int i = 0; i < 100; ++i) {
        myVector.push_back(i);
        stdVector.push_back(i);
    }

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试resize函数
TEST(IstlVectorTest, Resize) {
    istl::vector<int> myVector(10, 5);
    std::vector<int> stdVector(10, 5);

    myVector.resize(20, 10);
    stdVector.resize(20, 10);

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < myVector.size(); ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }

    myVector.resize(5);
    stdVector.resize(5);

    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < myVector.size(); ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

// 测试shrink_to_fit函数
TEST(IstlVectorTest, ShrinkToFit) {
    istl::vector<int> myVector(100, 1);
    std::vector<int> stdVector(100, 1);

    myVector.resize(50);
    stdVector.resize(50);

    myVector.shrink_to_fit();
    stdVector.shrink_to_fit();

    EXPECT_EQ(myVector.capacity(), stdVector.capacity());
    EXPECT_EQ(myVector.size(), stdVector.size());
    for (size_t i = 0; i < myVector.size(); ++i) {
        EXPECT_EQ(myVector[i], stdVector[i]);
    }
}

int IstlVectorTest(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void vectorTest(){
    istl::vector<int> myVector(5, 1);
    std::vector<int> stdVector(5, 1);

    myVector.insert(myVector.begin() + 2, 99);
    stdVector.insert(stdVector.begin() + 2, 99);

    myVector.insert(myVector.begin()+3, 20, 20);
    stdVector.insert(stdVector.begin()+3, 20, 20);


    for (size_t i = 0; i < myVector.size(); ++i) {
        //std::cout << myVector[i]<< " "<<  stdVector[i] << std::endl;
        if(myVector[i] != stdVector[i]) std::cout << "false";
    }
}