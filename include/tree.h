#ifndef TREE_HPP
#define TREE_HPP

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>

template <class TreeTrait> struct Node {
  using DataType = typename TreeTrait::DataType;
  using DataContainer = typename TreeTrait::DataContainer;
  using ChildrenContainer = typename TreeTrait::ChildrenContainer;

  DataContainer data;
  ChildrenContainer children;

  virtual typename DataContainer::const_iterator search(const DataType &value) const = 0;
  virtual void insert_in_node(int pos, const DataType &value) = 0;
  virtual void print() const = 0;
};

template <class TypeTrait>
class Tree {
public:
  static constexpr int TREE_ORDER = TypeTrait::TREE_ORDER;
  using DataType = typename TypeTrait::DataType;
  using ChildrenContainer = typename TypeTrait::ChildrenContainer;
  using DataContainer = typename TypeTrait::DataContainer;

  using NodeType = typename TypeTrait::Node;
  using NodePtr = typename std::shared_ptr<NodeType>;

  class iterator
      : public std::iterator<std::forward_iterator_tag, DataType, DataType,
                             const DataType *, const DataType &> {
    NodePtr root;
    NodePtr end;
    int count;

  public:
    iterator(NodePtr n = nullptr, int c = 0, NodePtr end = nullptr)
        : root(n), count(c), end(end) {}
    iterator &operator++() {
      if (root->count - 1 > count) {
        count++;
        return *this;
      }
      if (root == end)
        return nullptr;
      root = root->next;
      count = 0;
      return *this;
    }
    iterator operator++(int) {
      iterator it = *this;
      ++(*this);
      return it;
    }
    bool operator==(iterator it) const { return it.root == root; }
    bool operator!=(iterator it) const { return !(*this == it); }
    DataType operator*() { return root->data[count]; }
  };

protected:
  enum class state { BT_OVERFLOW, BT_UNDERFLOW, NORMAL };
  NodePtr root;

public:
  Tree(){};
  Tree(Tree &tree) = delete;
  ~Tree() = default;

  virtual void insert(const DataType &value) = 0;
  virtual state insert(NodePtr ptr, const DataType &value) = 0;

  virtual void split(NodePtr ptr, int pos) = 0;
  virtual void split_root(NodePtr ptr, const DataType &value) = 0;

  virtual void remove(const DataType &value) = 0;

  virtual void print() const = 0;
  virtual void print(NodePtr ptr, int level) const = 0;

  virtual iterator find(const DataType &value) const = 0;
  virtual iterator find(NodePtr ptr, const DataType &value) const = 0;

  iterator begin();
  iterator end();
};

#endif
