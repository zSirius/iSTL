// #include <gtest/gtest.h>
// #include <list>
// #include "list.h"  // 包含你实现的 istl::list 头文件

// // 测试默认构造函数
// TEST(IstlListTest, DefaultConstructor) {
//     istl::list<int> myList;
//     std::list<int> stdList;

//     EXPECT_EQ(myList.size(), stdList.size());
//     EXPECT_TRUE(myList.empty());
//     EXPECT_TRUE(stdList.empty());
// }

// // 测试大小构造函数
// TEST(IstlListTest, SizeConstructor) {
//     istl::list<int> myList(5);
//     std::list<int> stdList(5);
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();

//     EXPECT_EQ(myList.size(), stdList.size());
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试大小和初始值构造函数
// TEST(IstlListTest, SizeValueConstructor) {
//     istl::list<int> myList(5, 10);
//     std::list<int> stdList(5, 10);
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();

//     EXPECT_EQ(myList.size(), stdList.size());
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试push_back函数
// TEST(IstlListTest, PushBack) {
//     istl::list<int> myList;
//     std::list<int> stdList;

//     for (int i = 0; i < 10; ++i) {
//         myList.push_back(i);
//         stdList.push_back(i);
//     }

//     EXPECT_EQ(myList.size(), stdList.size());

//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试push_front函数
// TEST(IstlListTest, PushFront) {
//     istl::list<int> myList;
//     std::list<int> stdList;

//     for (int i = 0; i < 10; ++i) {
//         myList.push_front(i);
//         stdList.push_front(i);
//     }

//     EXPECT_EQ(myList.size(), stdList.size());
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试pop_back函数
// TEST(IstlListTest, PopBack) {
//     istl::list<int> myList;
//     std::list<int> stdList;

//     for (int i = 0; i < 10; ++i) {
//         myList.push_back(i);
//         stdList.push_back(i);
//     }

//     for (int i = 0; i < 5; ++i) {
//         myList.pop_back();
//         stdList.pop_back();
//     }

//     EXPECT_EQ(myList.size(), stdList.size());

//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试pop_front函数
// TEST(IstlListTest, PopFront) {
//     istl::list<int> myList;
//     std::list<int> stdList;

//     for (int i = 0; i < 10; ++i) {
//         myList.push_back(i);
//         stdList.push_back(i);
//     }

//     for (int i = 0; i < 5; ++i) {
//         myList.pop_front();
//         stdList.pop_front();
//     }

//     EXPECT_EQ(myList.size(), stdList.size());

//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试clear函数
// TEST(IstlListTest, Clear) {
//     istl::list<int> myList(5, 1);
//     std::list<int> stdList(5, 1);

//     myList.clear();
//     stdList.clear();

//     EXPECT_EQ(myList.size(), stdList.size());
//     EXPECT_TRUE(myList.empty());
//     EXPECT_TRUE(stdList.empty());
// }

// // 测试插入函数
// TEST(IstlListTest, Insert) {
//     istl::list<int> myList(5, 1);
//     std::list<int> stdList(5, 1);

//     myList.insert(myList.begin(), 99);
//     stdList.insert(stdList.begin(), 99);

//     myList.insert(++myList.begin(), 2, 42);
//     stdList.insert(++stdList.begin(), 2, 42);

//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();

//     EXPECT_EQ(myList.size(), stdList.size());
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试擦除函数
// TEST(IstlListTest, Erase) {
//     istl::list<int> myList(10, 1);
//     std::list<int> stdList(10, 1);

//     myList.erase(++myList.begin());
//     stdList.erase(++stdList.begin());

//     myList.erase(++myList.begin(), ----myList.end());
//     stdList.erase(++stdList.begin(), ----stdList.end());

//     EXPECT_EQ(myList.size(), stdList.size());
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试splice函数
// TEST(IstlListTest, Splice) {
//     istl::list<int> myList1(5, 1), myList2(3, 2);
//     std::list<int> stdList1(5, 1), stdList2(3, 2);

//     myList1.splice(myList1.begin(), myList2);
//     stdList1.splice(stdList1.begin(), stdList2);

//     EXPECT_EQ(myList1.size(), stdList1.size());
//     EXPECT_EQ(myList2.size(), stdList2.size());
    
//     auto myIt = myList1.begin();
//     auto stdIt = stdList1.begin();
//     for (; myIt != myList1.end() && stdIt != stdList1.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
//     myIt = myList2.begin();
//     stdIt = stdList2.begin();
//     for (; myIt != myList2.end() && stdIt != stdList2.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// //测试remove函数
// TEST(IstlListTest, Remove) {
//     istl::list<int> myList = {1, 2, 3, 1, 2, 3};
//     std::list<int> stdList = {1, 2, 3, 1, 2, 3};

//     myList.remove(2);
//     stdList.remove(2);

//     EXPECT_EQ(myList.size(), stdList.size());
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试unique函数
// TEST(IstlListTest, Unique) {
//     istl::list<int> myList = {1, 1, 2, 2, 3, 3};
//     std::list<int> stdList = {1, 1, 2, 2, 3, 3};

//     myList.unique();
//     stdList.unique();

//     EXPECT_EQ(myList.size(), stdList.size());
    
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试sort函数
// TEST(IstlListTest, Sort) {
//     istl::list<int> myList = {3, 1, 4, 1, 5, 9};
//     std::list<int> stdList = {3, 1, 4, 1, 5, 9};

//     myList.sort();
//     stdList.sort();

//     EXPECT_EQ(myList.size(), stdList.size());
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// // 测试reverse函数
// TEST(IstlListTest, Reverse) {
//     istl::list<int> myList = {1, 2, 3, 4, 5};
//     std::list<int> stdList = {1, 2, 3, 4, 5};

//     myList.reverse();
//     stdList.reverse();

//     EXPECT_EQ(myList.size(), stdList.size());
//     auto myIt = myList.begin();
//     auto stdIt = stdList.begin();
//     for (; myIt != myList.end() && stdIt != stdList.end(); ++myIt, ++stdIt) {
//         EXPECT_EQ(*myIt, *stdIt);
//     }
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

// /*
// 这个测试代码涵盖了以下  istl::list  和  std::list  的主要功能：
// 1. 构造函数（默认构造、大小构造、大小和初始值构造）
// 2. 基本元素操作（ push_back 、 push_front 、 pop_back 、 pop_front 、 clear ）
// 3. 元素插入和删除（ insert 、 erase ）
// 4. 链表操作（ splice 、 remove 、 unique 、 sort 、 reverse ）

// 运行这些测试可以验证你的  istl::list  实现与标准库  std::list  的行为一致性
// */