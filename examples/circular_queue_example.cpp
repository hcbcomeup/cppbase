#include <iostream>

#include "cppbase/circular_queue.hpp"

void TestBasicPushPop() {
  std::cout << "=== Test CircularQueue Basic Push/Pop ===" << std::endl;

  cppbase::CircularQueue<int> queue(5);

  std::cout << "IsEmpty: " << queue.IsEmpty() << std::endl;
  std::cout << "IsFull: " << queue.IsFull() << std::endl;

  for (int i = 1; i <= 5; ++i) {
    queue.PushBack(i * 10);
  }
  std::cout << "After pushing 5 elements, IsFull: " << queue.IsFull()
            << std::endl;
  std::cout << "Size: " << queue.GetSize() << std::endl;

  std::cout << "Front: " << queue.Front() << std::endl;
  queue.PopFront();
  std::cout << "After pop, Size: " << queue.GetSize() << std::endl;

  queue.PushBack(60);
  std::cout << "After overwrite, Front: " << queue.Front() << std::endl;
  std::cout << "OverRunCounter: " << queue.GetOverRunCounter() << std::endl;

  std::cout << std::endl;
}

void TestCircularOverwrite() {
  std::cout << "=== Test Circular Overwrite ===" << std::endl;

  cppbase::CircularQueue<int> queue(3);

  queue.PushBack(1);
  queue.PushBack(2);
  queue.PushBack(3);

  std::cout << "After pushing 3 elements, Size: " << queue.GetSize()
            << std::endl;
  std::cout << "OverRunCounter: " << queue.GetOverRunCounter() << std::endl;

  std::cout << "Front: " << queue.Front() << std::endl;
  queue.PopFront();

  std::cout << "After pop, Front: " << queue.Front() << std::endl;
  queue.PopFront();

  std::cout << "IsEmpty: " << queue.IsEmpty() << std::endl;

  std::cout << std::endl;
}

void TestMoveConstructor() {
  std::cout << "=== Test Move Constructor ===" << std::endl;

  cppbase::CircularQueue<int> queue1(5);
  queue1.PushBack(10);
  queue1.PushBack(20);

  cppbase::CircularQueue<int> queue2(std::move(queue1));

  std::cout << "Queue2 Size: " << queue2.GetSize() << std::endl;
  std::cout << "Queue2 Front: " << queue2.Front() << std::endl;

  std::cout << std::endl;
}

int main() {
  TestBasicPushPop();
  TestCircularOverwrite();
  TestMoveConstructor();

  std::cout << "All tests completed!" << std::endl;
  return 0;
}