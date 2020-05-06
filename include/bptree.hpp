#ifndef BPTREE_HPP
#define BPTREE_HPP

#include "tree.h"
#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>


template <class TypeTrait>
class BPTree : Tree<TypeTrait> {
public:
  using DataType = typename TypeTrait::DataType;
  using ChildrenContainer = typename TypeTrait::ChildrenContainer;
  using DataContainer = typename TypeTrait::DataContainer;

  using NodeType = typename TypeTrait::Node;
  using NodePtr = typename std::shared_ptr<NodeType>;

public:
  BPTree();
  BPTree(BPTree &tree) = delete;
  BPTree operator=(BPTree &tree) = delete;
  ~BPTree() = default;

  void insert(const DataType &value) override;
	typename Tree<TypeTrait>::state insert(NodePtr ptr, const DataType &value) override;

  void split(NodePtr ptr, int pos) override;
  void split_root(NodePtr ptr, const DataType &value) override;

  void remove(const DataType &value) override;

  void print() const override;
  void print(NodePtr ptr, int level) const override;

	typename Tree<TypeTrait>::iterator find(const DataType &value) const override;
  typename Tree<TypeTrait>::iterator find(NodePtr ptr, const DataType &value) const override;

  typename Tree<TypeTrait>::iterator begin();
  typename Tree<TypeTrait>::iterator end();
};

#endif
