#ifndef UTEC_BTREE_HPP
#define UTEC_BTREE_HPP
#include <array>
#include <memory>
#include <optional>
#include <type_traits>
#include <functional>

template <class Trait>
struct Node {
  static constexpr int BTREE_ORDER = Trait::BTREE_ORDER;

  using DataType = typename Trait::DataType;
	using ChildrenContainer = typename Trait::ChildrenContainer;
	//using ChildrenContainer = typename std::array<NodePtr, BTREE_ORDER + 2>
	using DataContainer = typename Trait::DataContainer;
	using Iterator = typename Trait::Iterator;
  using NodePtr = typename std::shared_ptr<Node>;

	DataContainer data;
	inthildrenContainer children;

  Node();
  Node(Node& node) = default;
  Node& operator=(Node& node) = default;
  ~Node() = default;

	void insert_in_node(int pos, const DataType& value){
		static_cast<Derived const&>(*this).insert_helper(pos, data,children, value);
	}

	Iterator search(const DataType& value){
		search_helper<Node<Trait>>(data, value);
	}
	void print(){
		print_helper<Node<Trait>>(data);
	}

  bool is_overflow() const {
		return false;
	}
};

template <class NodeType> class BTree {
public:
  static constexpr int BTREE_ORDER = Trait::BTREE_ORDER;
  using DataType = typename Trait::DataType;
	using DataContainer = typename Trait::DataContainer;
	
  using Node = typename ::Node<Trait>;
  using NodePtr = typename std::shared_ptr<Node>;
	using ChildrenContainer = typename Trait::ChildrenContainer;

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
