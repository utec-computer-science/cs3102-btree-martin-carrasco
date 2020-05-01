#include "btree.cpp"
#include "../include/btree_traits.hpp"

int main(){
	using Trait = BTreeTrait<int>;
	BTree<Trait> btree_3;
	btree_3.insert(1);
	btree_3.insert(2);
	btree_3.insert(3);
	btree_3.insert(4);

	btree_3.print();

	return 0;

}
