#include "../include/btree.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>



// BTree

template <class TypeTrait>
typename BTree<TypeTrait>::iterator BTree<TypeTrait>::begin() {
  return iterator(root);
}

template <class TypeTrait>
typename BTree<TypeTrait>::iterator BTree<TypeTrait>::end() {
  return iterator(nullptr);
}

template <class TypeTrait> BTree<TypeTrait>::BTree() {
  root = std::make_shared<NodeType>();
}

template <class TypeTrait>
void BTree<TypeTrait>::insert(const DataType &value) {
  auto current_state = insert(root, value);

  if (current_state == state::BT_OVERFLOW)
    split_root(root, value);
}

template <class TypeTrait>
typename BTree<TypeTrait>::state
BTree<TypeTrait>::insert(NodePtr ptr, const DataType &value) {

  // Find optimized
  /*auto it =
      std::find_if(ptr->data.begin(), ptr->data.end(),
                   [&value](const auto& current_value) { return value >=
     current_value; });

        std::cout << it << std::endl;
        std::cout << *it << std::endl;
        std::ptrdiff_t pos = std::distance(ptr->data.begin(), it);
        std::cout << pos << std::endl << "------------" << std::endl;*/

  int pos = 0;

  while (pos < ptr->count && ptr->data[pos] < value) {
    pos++;
  }

  if (ptr->children[pos] != nullptr) {
    auto current_state = insert(ptr->children[pos], value);
    if (current_state == state::BT_OVERFLOW)
      split(ptr, pos);
  } else
    ptr->insert_in_node(pos, value);

  return ptr->is_overflow() == true ? state::BT_OVERFLOW : state::NORMAL;
}

template <class TypeTrait> void BTree<TypeTrait>::split(NodePtr ptr, int pos) {

  constexpr int half_order = BTREE_ORDER / 2;
  NodePtr node_in_overflow = ptr->children[pos];
  NodePtr child1 = std::make_shared<NodeType>();
  NodePtr child2 = std::make_shared<NodeType>();

  // Copy elements from overflow node into child node left and update count
  std::copy_n(node_in_overflow->children.begin(), half_order + 1,
              child1->children.begin());
  std::copy_n(node_in_overflow->data.begin(), half_order, child1->data.begin());
  child1->count = half_order;

  ptr->insert_in_node(pos, node_in_overflow->data[half_order]);

  // Copy elements from overflow node into child node right and update count
  std::copy_n(node_in_overflow->children.begin() + half_order + 1 + 1,
              half_order, child2->children.begin());
  std::copy_n(node_in_overflow->data.begin() + half_order + 1, half_order,
              child2->data.begin());
  child2->count = half_order;

  // Replace parent children
  ptr->children[pos] = child1;
  ptr->children[pos + 1] = child2;
}

template <class TypeTrait>
void BTree<TypeTrait>::split_root(NodePtr ptr, const DataType &value) {
  std::cout << "entro" << std::endl;
  constexpr int half_order = BTREE_ORDER / 2;

  NodePtr node_in_overflow = ptr;
  NodePtr lhc = std::make_shared<NodeType>();
  NodePtr rhc = std::make_shared<NodeType>();

  // Copy children and data to left hand children
  std::copy_n(node_in_overflow->children.begin(), half_order,
              lhc->children.begin());
  std::copy_n(node_in_overflow->data.begin(), half_order, lhc->data.begin());
  lhc->count = half_order;

  // the middle element

  // Copy children and data to right hand children
  std::copy_n(node_in_overflow->children.begin() + half_order + 1,
              half_order + 1, rhc->children.begin());
  std::copy_n(node_in_overflow->data.begin() + half_order + 1, half_order + 1,
              rhc->data.begin());
  rhc->count = half_order + 1;

  ptr->children[0] = lhc;
  ptr->data[0] = node_in_overflow->data[half_order];
  ptr->children[1] = rhc;
  ptr->count = 1;
}

template <class TypeTrait>
typename BTree<TypeTrait>::iterator
BTree<TypeTrait>::find(const DataType &value) const {
  return find(root, value);
}

template <class TypeTrait>
typename BTree<TypeTrait>::iterator
BTree<TypeTrait>::find(NodePtr ptr, const DataType &value) const {
  if (ptr == nullptr)
    return iterator(nullptr);

  int pos = 0;
  while (pos < ptr->count && ptr->data[pos] < value) {
    pos++;
  }

  if (ptr->data[pos] == value)
    return iterator(ptr, pos);
  if (ptr->children[pos])
    return find(ptr->children[pos], value);
  return iterator(nullptr);
}

template <class TypeTrait> void BTree<TypeTrait>::print() const {
  print(root, 0);
  std::cout << "________________________\n";
}

template <class TypeTrait>
void BTree<TypeTrait>::print(NodePtr ptr, int level) const {
  // TODO: Optimize
  if (ptr == nullptr) {
    return;
  }
  int i;
  for (i = ptr->count - 1; i >= 0; i--) {
    print(ptr->children[i + 1], level + 1);
    for (int k = 0; k < level; k++) {
      std::cout << "    ";
    }
    std::cout << ptr->data[i] << std::endl;
  }
  print(ptr->children[i + 1], level + 1);
}
