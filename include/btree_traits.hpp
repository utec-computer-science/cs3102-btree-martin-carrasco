#ifndef BTREE_TRAITS_HPP
#define BTREE_TRAITS_HPP

#include "btree.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

template <class Trait> struct BTreeNode : public Node<Trait> {
  int count = 0;
  using DataType = typename Trait::DataType;
  using ChildrenContainer = typename Trait::ChildrenContainer;
  using DataContainer = typename Trait::DataContainer;
  static constexpr int BTREE_ORDER = Trait::TREE_ORDER;

  bool is_overflow() const { return count > BTREE_ORDER; }

	typename DataContainer::const_iterator search(const DataType &value) const override {
		return std::find(std::begin(this->data), std::end(this->data), value);
  }

  void print() const override {
    std::for_each(this->data.cbegin(), this->data.cend(),
                  [](const auto &value) { std::cout << value; });
  }

  void insert_in_node(int pos, const DataType &value) override {
    auto j = count;
    while (j > pos) {
      this->data[j] = this->data[j - 1];
      this->children[j + 1] = this->children[j];
      j--;
    }
    this->data[j] = value;
    this->children[j + 1] = this->children[j];
    this->count++;
  }

  BTreeNode() {
    std::fill(this->children.begin(), this->children.end(), nullptr);
  }
};

template <class T, int Order = 3> struct BTreeTrait {
	static constexpr int TREE_ORDER = 3;
  using DataType = T;
  using DataContainer = typename std::array<T,TREE_ORDER  + 1>;
  using Node = BTreeNode<BTreeTrait<T>>;
  using ChildrenContainer =
      typename std::array<std::shared_ptr<Node>, TREE_ORDER + 2>;
};

#endif
