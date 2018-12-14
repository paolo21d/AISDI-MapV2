#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include<chrono>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{
template <typename K, typename V>
using Tree = aisdi::TreeMap<K, V>;
template <typename K, typename V>
using Hash = aisdi::HashMap<K, V>;
/*void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}*/
void testTree(const std::size_t repeatCount) {
	Tree<int, int> treeMap;
	auto start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap[i] = i;
	}
	auto done = std::chrono::system_clock::now();
	std::cout << "TreeMap dodanie elementow: " << (done - start).count() << '\n';
	start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap.find(i);
	}
	done = std::chrono::system_clock::now();
	std::cout << "TreeMap wyszukiwanie elementow: " << (done - start).count() << '\n';
}
void testHash(const std::size_t repeatCount) {
	Hash<int, int> hashMap;
	auto start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap[i] = i;
	}
	auto done = std::chrono::system_clock::now();
	std::cout << "HashMap dodanie elementow: " << (done - start).count() << '\n';
	start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap.find(i);
	}
	done = std::chrono::system_clock::now();
	std::cout << "HashMap wyszukiwanie elementow: " << (done - start).count() << '\n';
}
} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1000000;
  /*for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();*/


  testTree(repeatCount);
  testHash(repeatCount);
	/*aisdi::TreeMap<int, int> treeMap;
	aisdi::HashMap<int, int> hashMap;

	//auto start = std::chrono::system_clock::now();
	//auto done = std::chrono::system_clock::now();
	//std::cout << "LinkedList preppend time: " << (done - start).count() << '\n';

	//Dodanie do TreeMap
	auto start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap[i] = i;
	}
	auto done = std::chrono::system_clock::now();
	std::cout << "TreeMap dodanie elementow: " << (done - start).count() << '\n';
	//Dodanie do HashMap
	start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap[i] = i;
	}
	done = std::chrono::system_clock::now();
	std::cout << "HashMap dodanie elementow: " << (done - start).count() << '\n';


	///Wyszukiwanie w TreeMap
	start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap.find(i);
	}
	done = std::chrono::system_clock::now();
	std::cout << "TreeMap wyszukiwanie elementow: " << (done - start).count() << '\n';
	///Wyszukiwanie w HashMap
	start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap.find(i);
	}
	done = std::chrono::system_clock::now();
	std::cout << "HashMap wyszukiwanie elementow: " << (done - start).count() << '\n';*/

}
