#include "primer/trie.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <string_view>
#include <vector>
#include "common/exception.h"

namespace bustub {
std::vector<const TrieNode *> Trie::findPath(std::string_view key) const {
  // key is not end with '\0'
  std::vector<const TrieNode *> path;
  const TrieNode *tempNode = root_.get();
  if (tempNode != nullptr) {
    path.emplace_back(tempNode);
    for (auto c : key) {
      if (auto it = tempNode->children_.find(c); it != tempNode->children_.end()) {
        tempNode = it->second.get();
        path.emplace_back(tempNode);
      } else {
        break;
      }
    }
  }
  return path;
}
template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.

  // maybe root
  if (key.size() > 0 && key.back() == '\0') key = key.substr(0, key.size() - 1);
  auto path = findPath(key);
  if (path.size() != key.size() + 1) return nullptr;
  if (auto ptr = dynamic_cast<const TrieNodeWithValue<T> *>(path.back()); ptr == nullptr)
    return nullptr;
  else
    return ptr->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  if (key.size() > 0 && key.back() == '\0') key = key.substr(0, key.size() - 1);
  std::vector<std::shared_ptr<TrieNode>> newNodes;
  auto path = findPath(key);
  // path.size()<=key.size()+1
  for (size_t index = 0; index < key.size(); ++index) {
    if (path.size() > index) {
      newNodes.emplace_back(std::shared_ptr<TrieNode>(path[index]->Clone()));
    } else {
      newNodes.emplace_back(std::make_shared<TrieNode>());
    }
  }
  if (path.size() == key.size() + 1) {
    newNodes.emplace_back(
        std::make_shared<TrieNodeWithValue<T>>(path.back()->children_, std::make_shared<T>(std::move(value))));
  } else {
    newNodes.emplace_back(std::make_shared<TrieNodeWithValue<T>>(std::make_shared<T>(std::move(value))));
  }
  for (size_t index = newNodes.size() - 1; index > 0; --index) {
    newNodes[index - 1]->children_.insert_or_assign(key[index - 1], std::move(newNodes[index]));
  }
  // newNodes must have one
  return Trie(newNodes[0]);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.

  // if not found key
  if (key.size() > 0 && key.back() == '\0') key = key.substr(0, key.size() - 1);
  auto path = findPath(key);
  if (path.size() != key.size() + 1 || path.back()->is_value_node_ == false) {
    return Trie(root_);
  }
  // newNode number needed
  size_t sum = path.size();
  if (path[sum - 1]->children_.empty()) {
    --sum;
    for (; sum > 0; --sum) {
      if (path[sum - 1]->is_value_node_ == false && path[sum - 1]->children_.size() == 1) {
      } else {
        break;
      }
    }
  }

  if (sum <= 0) {
    return Trie();
  } else {
    std::vector<std::shared_ptr<TrieNode>> newNodes;
    for (size_t index = 0; index < sum - 1; ++index) {
      newNodes.emplace_back(std::shared_ptr<TrieNode>(path[index]->Clone()));
    }
    // need to save the last node
    if (sum == key.size() + 1)
      newNodes.emplace_back(std::make_shared<TrieNode>(path[sum - 1]->children_));
    else {
      // don't need to save
      newNodes.emplace_back(std::shared_ptr<TrieNode>(path[sum - 1]->Clone()));
      newNodes.back()->children_.erase(key[sum - 1]);
    }
    for (size_t index = sum - 1; index > 0; --index) {
      newNodes[index - 1]->children_.insert_or_assign(key[index - 1], std::move(newNodes[index]));
    }
    return Trie(std::move(newNodes[0]));
  }
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
