// 测试pop_back函数
// TEST(IstlDequeTest, PopBack) {
//     istl::deque<int> myDeque;
//     std::deque<int> stdDeque;

//     for (int i = 0; i < 10; ++i) {
//         myDeque.push_back(i);
//         stdDeque.push_back(i);
//     }

//     for (int i = 0; i < 5; ++i) {
//         myDeque.pop_back();
//         stdDeque.pop_back();
//     }

//     EXPECT_EQ(myDeque.size(), stdDeque.size());
//     for (int i = 0; i < 5; ++i) {
//         EXPECT_EQ(myDeque[i], stdDeque[i]);
//     }
// }

// // 测试pop_front函数
// TEST(IstlDequeTest, PopFront) {
//     istl::deque<int> myDeque;
//     std::deque<int> stdDeque;

//     for (int i = 0; i < 10; ++i) {
//         myDeque.push_back(i);
//         stdDeque.push_back(i);
//     }

//     for (int i = 0; i < 5; ++i) {
//         myDeque.pop_front();
//         stdDeque.pop_front();
//     }

//     EXPECT_EQ(myDeque.size(), stdDeque.size());
//     for (int i = 0; i < 5; ++i) {
//         EXPECT_EQ(myDeque[i], stdDeque[i]);
//     }
// }

// // 测试clear函数
// TEST(IstlDequeTest, Clear) {
//     istl::deque<int> myDeque(5, 1);
//     std::deque<int> stdDeque(5, 1);

//     myDeque.clear();
//     stdDeque.clear();

//     EXPECT_EQ(myDeque.size(), stdDeque.size());
//     EXPECT_TRUE(myDeque.empty());
//     EXPECT_TRUE(stdDeque.empty());
// }

// // 测试resize函数
// TEST(IstlDequeTest, Resize) {
//     istl::deque<int> myDeque(10, 5);
//     std::deque<int> stdDeque(10, 5);

//     myDeque.resize(20, 10);
//     stdDeque.resize(20, 10);

//     EXPECT_EQ(myDeque.size(), stdDeque.size());
//     for (size_t i = 0; i < myDeque.size(); ++i) {
//         EXPECT_EQ(myDeque[i], stdDeque[i]);
//     }

//     myDeque.resize(5);
//     stdDeque.resize(5);

//     EXPECT_EQ(myDeque.size(), stdDeque.size());
//     for (size_t i = 0; i < myDeque.size(); ++i) {
//         EXPECT_EQ(myDeque[i], stdDeque[i]);
//     }
// }

// // 测试swap函数
// TEST(IstlDequeTest, Swap) {
//     istl::deque<int> myDeque1(5, 1);
//     istl::deque<int> myDeque2(10, 2);
//     std::deque<int> stdDeque1(5, 1);
//     std::deque<int> stdDeque2(10, 2);

//     myDeque1.swap(myDeque2);
//     stdDeque1.swap(stdDeque2);

//     EXPECT_EQ(myDeque1.size(), stdDeque1.size());
//     EXPECT_EQ(myDeque2.size(), stdDeque2.size());
//     for (size_t i = 0; i < myDeque1.size(); ++i) {
//         EXPECT_EQ(myDeque1[i], stdDeque1[i]);
//     }
//     for (size_t i = 0; i < myDeque2.size(); ++i) {
//         EXPECT_EQ(myDeque2[i], stdDeque2[i]);
//     }
// }