// #include <gtest/gtest.h>
// #include <string>
// #include "istl_string.h"  // 包含你实现的 istl::string 头文件

// // 测试默认构造函数
// TEST(IstlStringTest, DefaultConstructor) {
//     istl::string myString;
//     std::string stdString;

//     EXPECT_EQ(myString.size(), stdString.size());
//     EXPECT_TRUE(myString.empty());
//     EXPECT_TRUE(stdString.empty());
// }

// void test2string(istl::string &myString, std::string &stdString){
//     EXPECT_EQ(myString.size(), stdString.size());
    
//     size_t size = myString.size();
//     const char* s1 = myString.c_str(), *s2 = stdString.c_str();
//     for(int i=0; i<size; i++){
//         EXPECT_EQ(s1[i], s2[i]);
//     }
// }

// // 测试字符串构造函数
// TEST(IstlStringTest, CStrConstructor) {
//     const char* testStr = "hello world";
//     istl::string myString(testStr);
//     std::string stdString(testStr);
//     test2string(myString, stdString);
// }

// // 测试复制构造函数
// TEST(IstlStringTest, CopyConstructor) {
//     istl::string myString("hello world");
//     istl::string myStringCopy(myString);

//     std::string stdString("hello world");
//     std::string stdStringCopy(stdString);

//     test2string(myStringCopy, stdStringCopy);
// }

// // 测试移动构造函数
// TEST(IstlStringTest, MoveConstructor) {
//     istl::string myString("hello world");
//     istl::string myStringMove(std::move(myString));

//     std::string stdString("hello world");
//     std::string stdStringMove(std::move(stdString));

//     test2string(myStringMove, stdStringMove);
// }

// // 测试赋值操作符
// TEST(IstlStringTest, AssignmentOperator) {
//     istl::string myString = "hello";
//     istl::string myStringAssign;
//     myStringAssign = myString;

//     std::string stdString = "hello";
//     std::string stdStringAssign;
//     stdStringAssign = stdString;

//     test2string(myStringAssign, stdStringAssign);
// }

// // 测试移动赋值操作符
// TEST(IstlStringTest, MoveAssignmentOperator) {
//     istl::string myString = "hello";
//     istl::string myStringMoveAssign;
//     myStringMoveAssign = std::move(myString);

//     std::string stdString = "hello";
//     std::string stdStringMoveAssign;
//     stdStringMoveAssign = std::move(stdString);

//     test2string(myStringMoveAssign, stdStringMoveAssign);
// }

// // 测试前后访问操作
// TEST(IstlStringTest, FrontBack) {
//     istl::string myString = "hello";
//     std::string stdString = "hello";

//     EXPECT_EQ(myString.front(), stdString.front());
//     EXPECT_EQ(myString.back(), stdString.back());
// }

// // 测试迭代器
// TEST(IstlStringTest, Iterators) {
//     istl::string myString = "hello";
//     std::string stdString = "hello";

//     auto myit = myString.begin();
//     auto stdit = stdString.begin();
//     while(myit != myString.end() && stdit != stdString.end()){
//         EXPECT_EQ((*myit), *(stdit));
//         ++myit, ++stdit;
//     }
    
// }

// // 测试容量相关操作
// TEST(IstlStringTest, Capacity) {
//     istl::string myString = "hello";
//     std::string stdString = "hello";

//     EXPECT_EQ(myString.size(), stdString.size());
//     EXPECT_EQ(myString.length(), stdString.length());
//     EXPECT_EQ(myString.capacity(), stdString.capacity());
//     EXPECT_EQ(myString.empty(), stdString.empty());
// }

// // 测试清空操作
// TEST(IstlStringTest, Clear) {
//     istl::string myString = "hello";
//     std::string stdString = "hello";

//     myString.clear();
//     stdString.clear();

//     EXPECT_EQ(myString.size(), stdString.size());
//     EXPECT_TRUE(myString.empty());
//     EXPECT_TRUE(stdString.empty());
// }

// // 测试字符串拼接
// TEST(IstlStringTest, Append) {
//     istl::string myString = "hello";
//     myString.append(" world");

//     std::string stdString = "hello";
//     stdString.append(" world");

//     test2string(myString, stdString);
// }

// // 测试插入操作
// TEST(IstlStringTest, Insert) {
//     istl::string myString = "hello";
//     myString.insert(5, " world");

//     std::string stdString = "hello";
//     stdString.insert(5, " world");

//     test2string(myString, stdString);
// }

// // 测试擦除操作
// TEST(IstlStringTest, Erase) {
//     istl::string myString = "hello world";
//     myString.erase(5, 6);

//     std::string stdString = "hello world";
//     stdString.erase(5, 6);

//     test2string(myString, stdString);

// }

// // 测试替换操作
// TEST(IstlStringTest, Replace) {
//     istl::string myString = "hello world";
//     myString.replace(6, 5, "there");

//     std::string stdString = "hello world";
//     stdString.replace(6, 5, "there");

//     test2string(myString, stdString);

//     std::cout << "replace:" << myString << " " << stdString << std::endl;
// }

// // 测试查找操作
// TEST(IstlStringTest, Find) {
//     istl::string myString = "hello world";
//     size_t pos = myString.find("world");

//     std::string stdString = "hello world";
//     size_t stdPos = stdString.find("world");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试反向查找操作
// TEST(IstlStringTest, RFind) {
//     istl::string myString = "hello world world";
//     size_t pos = myString.rfind("world");

//     std::string stdString = "hello world world";
//     size_t stdPos = stdString.rfind("world");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试子字符串查找操作
// TEST(IstlStringTest, FindFirstOf) {
//     istl::string myString = "hello world";
//     size_t pos = myString.find_first_of("world");

//     std::string stdString = "hello world";
//     size_t stdPos = stdString.find_first_of("world");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试子字符串查找操作
// TEST(IstlStringTest, FindLastOf) {
//     istl::string myString = "hello world";
//     size_t pos = myString.find_last_of("world");

//     std::string stdString = "hello world";
//     size_t stdPos = stdString.find_last_of("world");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试子字符串查找操作
// TEST(IstlStringTest, FindFirstNotOf) {
//     istl::string myString = "hello world";
//     size_t pos = myString.find_first_not_of("helo");

//     std::string stdString = "hello world";
//     size_t stdPos = stdString.find_first_not_of("helo");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试子字符串查找操作
// TEST(IstlStringTest, FindLastNotOf) {
//     istl::string myString = "hello world";
//     size_t pos = myString.find_last_not_of("drlow");

//     std::string stdString = "hello world";
//     size_t stdPos = stdString.find_last_not_of("drlow");

//     EXPECT_EQ(pos, stdPos);
// }

// // 测试比较操作
// TEST(IstlStringTest, Compare) {
//     istl::string myString = "hello";
//     int result = myString.compare("hello");

//     std::string stdString = "hello";
//     int stdResult = stdString.compare("hello");

//     EXPECT_EQ(result, stdResult);
// }

// // 测试交换操作
// TEST(IstlStringTest, Swap) {
//     istl::string myString1 = "hello";
//     istl::string myString2 = "world";

//     std::string stdString1 = "hello";
//     std::string stdString2 = "world";

//     myString1.swap(myString2);
//     stdString1.swap(stdString2);

//     test2string(myString1, stdString1);
//     test2string(myString2, stdString2);
// }


// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     ::testing::GTEST_FLAG(filter) = "IstlStringTest.*";
//     return RUN_ALL_TESTS();
// }