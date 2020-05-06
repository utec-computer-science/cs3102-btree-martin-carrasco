
#include "../include/bptree.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>


template <class TypeTrait>
void BPTree<TypeTrait>::remove(const DataType &value){

}
template <class TypeTrait>
typename Tree<TypeTrait>::iterator BPTree<TypeTrait>::begin() {
  return iterator(this->root);
}

template <class TypeTrait>
typename Tree<TypeTrait>::iterator BPTree<TypeTrait>::end() {
  return Tree<TypeTrait>::iterator(nullptr);
}

template <class TypeTrait> BPTree<TypeTrait>::BPTree() : Tree<TypeTrait>() {
  this->root = std::make_shared<NodeType>();
}

template <class TypeTrait>
void BPTree<TypeTrait>::insert(const DataType &value) {
  auto current_state = insert(this->root, value);

  if (current_state == Tree<TypeTrait>::state::BT_OVERFLOW)
    split_root(this->root, value);
}

template <class TypeTrait>
typename Tree<TypeTrait>::state
BPTree<TypeTrait>::insert(NodePtr ptr, const DataType &value) {

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
    if (current_state == Tree<TypeTrait>::state::BT_OVERFLOW)
      split(ptr, pos);
  } else
    ptr->insert_in_node(pos, value);

  return ptr->is_overflow() == true ? Tree<TypeTrait>::state::BT_OVERFLOW : Tree<TypeTrait>::state::NORMAL;
}

template <class TypeTrait> void BPTree<TypeTrait>::split(NodePtr ptr, int pos) {

  constexpr int half_order = this->TREE_ORDER / 2;
  NodePtr node_in_overflow = ptr->children[pos];
  NodePtr child1 = std::make_shared<NodeType>();
  NodePtr child2 = std::make_shared<NodeType>();

	child2->is_leaf = true;
	child1->next = child2;
	child2->prev = child1;


  // Copy elements from overflow node into child node left and update count
  std::copy_n(node_in_overflow->children.begin(), half_order + 1,
              child1->children.begin());
  std::copy_n(node_in_overflow->data.begin(), half_order, child1->data.begin());
  child1->count = half_order;

  ptr->insert_in_node(pos, node_in_overflow->data[half_order]);

	int copy_middle = not node_in_overflow->is_leaf ? 1 : 0;

  // Copy elements from overflow node into child node right and update count
  std::copy_n(node_in_overflow->children.begin() + half_order + 1 + copy_middle,
              half_order, child2->children.begin());
  std::copy_n(node_in_overflow->data.begin() + half_order + copy_middle, half_order,
              child2->data.begin());
  child2->count = half_order;

  // Replace parent children
	ptr->leaf = false;
  ptr->children[pos] = child1;
  ptr->children[pos + 1] = child2;
}

template <class TypeTrait>
void BPTree<TypeTrait>::split_root(NodePtr ptr, const DataType &value) {
  std::cout << "entro" << std::endl;
  constexpr int half_order = this->TREE_ORDER / 2;

  NodePtr node_in_overflow = ptr;
  NodePtr lhc = std::make_shared<NodeType>();
  NodePtr rhc = std::make_shared<NodeType>();

	if(not ptr->is_leaf){
		lhc->is_leaf = false;
		rhc->is_leaf = false;
	}

	lhc->next = rhc;
	rhc->prev = lhc;

  // Copy children and data to left hand children
  std::copy_n(node_in_overflow->children.begin(), half_order,
              lhc->children.begin());
  std::copy_n(node_in_overflow->data.begin(), half_order, lhc->data.begin());
  lhc->count = half_order;

	if(not ptr->is_leaf){
		ptr->data[0] = node_in_overflow->data[half_order];
	}
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
typename Tree<TypeTrait>::iterator
BPTree<TypeTrait>::find(const DataType &value) const {
  return find(this->root, value);
}

template <class TypeTrait>
typename Tree<TypeTrait>::iterator
BPTree<TypeTrait>::find(NodePtr ptr, const DataType &value) const {
  if (ptr == nullptr)
    return typename Tree<TypeTrait>::iterator(nullptr);

  int pos = 0;
  while (pos < ptr->count && ptr->data[pos] < value) {
    pos++;
  }

  if (ptr->data[pos] == value)
    return typename Tree<TypeTrait>::iterator(ptr, pos);
  if (ptr->children[pos])
    return find(ptr->children[pos], value);
  return typename Tree<TypeTrait>::iterator(nullptr);
}

template <class TypeTrait> void BPTree<TypeTrait>::print() const {
  print(this->root, 0);
  std::cout << "________________________\n";
}

template <class TypeTrait>
void BPTree<TypeTrait>::print(NodePtr ptr, int level) const  {
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
