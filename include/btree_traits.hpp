#ifndef BTREE_TRAITS_HPP
#define BTREE_TRAITS_HPP

#include "btree.hpp"
#include <array>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>



template <class T>
struct Order3Int {
	static constexpr int ORDER = 3;
	using DataType = T;
};


template <class Attributes, class DataContainer, class ChildrenContainer>
struct BTreeTrait{
	int count = 0;
  static constexpr int BTREE_ORDER = Attributes::ORDER;
	using DataType = typename Attributes::DataType;
	using Iterator = typename DataContainer::iterator;

	Iterator search_helper(const DataContainer& container, const DataType& value){
			return std::find_if(container.begin(), container.end(), value);
	}

	void print_helper(const DataContainer& container) {
		std::for_each(container.begin(), container.end(), [](const auto& value){std::cout << value;});
	}

	void insert(const int& pos, int& count, const DataType& value ,const DataContainer& data, const ChildrenContainer& children){

		auto j = count;
		while (j > pos) {
			data[j] = data[j - 1];
			children[j + 1] = children[j];
			j--;
		}
		data[j] = value;
		children[j + 1] = children[j];
		count++;
	}

	bool simple_is_overflown() const {
		return count > BTREE_ORDER;
	}
};
#endif
