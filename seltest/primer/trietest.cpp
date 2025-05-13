#include <iostream>
#include "../../src/include/primer/trie.h"
int main() {
  bustub::Trie trie;
  auto trie1 = trie.Put<uint32_t>("hu", 20);
  auto trie2 = trie.Put<uint32_t>("he", 30);
  auto trie3 = trie1.Put<uint32_t>("he", 30);
  auto trie4 = trie3.Remove("hu");
  std::cout << *trie1.Get<uint32_t>("hu");
}