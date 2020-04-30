#include "../include/btree.hpp"
#include "../include/btree_traits.hpp"

int main(){
	using AttributeTrait = typename Order3<int>;
	using NodeType = Node<AttributeTrait>;
	BTree<Trait, CRTP> btree_3;
	btree_3.insert(1);
	btree_3.insert(2);
	btree_3.insert(3);
	btree_3.insert(4);

	btree_3.print();

	return 0;

}
