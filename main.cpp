#include "vector.hpp"
#include <iostream>

void demonstrateArray() {
  custom::DynamicArray<int> numbers;

  for (int i = 0; i < 10; ++i) {
    numbers.append(i * 2);
  }

  std::cout << "Array contents: ";
  for (std::size_t i = 0; i < numbers.currentSize(); ++i) {
    std::cout << numbers.getElementAt(i) << " ";
  }
  std::cout << "\n";

  custom::DynamicArray<int> moved_array = std::move(numbers);
  std::cout << "After move - size: " << moved_array.currentSize()
            << ", capacity: " << moved_array.totalCapacity() << "\n";
}

void testInitializerList() {
  custom::DynamicArray<std::string> words = {"apple", "banana", "cherry"};

  std::cout << "Words: ";
  for (std::size_t i = 0; i < words.currentSize(); ++i) {
    std::cout << words.getElementAt(i) << " ";
  }
  std::cout << "\n";
}

void testCopyOperations() {
  custom::DynamicArray<double> original = {1.1, 2.2, 3.3};
  custom::DynamicArray<double> copy = original;

  std::cout << "Copy test: ";
  for (std::size_t i = 0; i < copy.currentSize(); ++i) {
    std::cout << copy.directAccess(i) << " ";
  }
  std::cout << "\n";
}

int main() {
  demonstrateArray();
  testInitializerList();
  testCopyOperations();
  return 0;
}
// additional changes for PR
