#ifndef UTEC_BTREE_HPP
#define UTEC_BTREE_HPP
#include <array>
#include <memory>
#include <optional>
#include <type_traits>
#include <functional>

template <class TreeTrait> struct Node {
  static constexpr int BTREE_ORDER = TreeTrait::BTREE_ORDER;

  using DataType = typename TreeTrait::DataType;
	using DataContainer = typename TreeTrait::DataContainer;
	using ChildrenContainer = typename TreeTrait::ChildrenContainer;
  using NodePtr = typename std::shared_ptr<Node<TreeTrait>>;

	DataContainer data;
	ChildrenContainer children;

	typename TreeTrait::search search;
	typename TreeTrait::print print;
	typename TreeTrait::insert insert;

  Node();
  Node(Node& node) = default;
  Node& operator=(Node& node) = default;
  ~Node() = default;

  void insert_in_node(int pos, const DataType &value){
		insert_inner(pos, value, data, children);
	}
  bool is_overflow() const;
};

template <class TypeTrait> class BTree {
public:
  static constexpr int BTREE_ORDER = TypeTrait::BTREE_ORDER;
  using DataType = typename TypeTrait::DataType;
	using ChildrenContainer = typename TypeTrait::ChildrenContainer;
	using DataContainer = typename TypeTrait::DataContainer;
	using NodeIterator = typename TypeTrait::iterator;
	
  using Node = typename ::Node<BTree<TypeTrait>>;
  using NodePtr = typename std::shared_ptr<Node>;

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

private:
  enum class state { BT_OVERFLOW, BT_UNDERFLOW, NORMAL };
  NodePtr root;

public:
  BTree();
  BTree(BTree& tree) = delete;
  BTree operator=(BTree& tree) = delete;
  ~BTree() = default;

  void insert(const DataType &value);
  state insert(NodePtr ptr, const DataType &value);

  void split(NodePtr ptr, int pos);
  void split_root(NodePtr ptr, const DataType &value);

  void remove(const DataType &value);

  void print() const;
  void print(NodePtr ptr, int level) const;

  iterator find(const DataType& value) const;
  iterator find(NodePtr ptr, const DataType& value) const;

  iterator begin();
  iterator end();
};

#endif
