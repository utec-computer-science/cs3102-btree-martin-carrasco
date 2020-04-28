#ifndef BTREE_TRAITS_HPP
#define BTREE_TRAITS_HPP

#include <array>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>


template <class ToEncapsulate, int size, int Flag>
struct children_converter{};

template <class ToEncapsulate, int size>
struct children_converter<ToEncapsulate, size, 1> {
	using value = std::array<ToEncapsulate, size>;

};



template <class T, class ChildrenConverter>
struct Order3Trait {
	int count = 0;
  static constexpr int BTREE_ORDER = 3;
	using DataType = T;
	using DataContainer = typename std::array<T, BTREE_ORDER + 1>;
	using ChildrenContainer = typename children_converter<T, BTREE_ORDER + 2, 1>::value;
	using iterator = typename DataContainer::iterator;

	iterator simple_search(const DataContainer& container, const T& value){
			return std::find_if(container.begin(), container.end(), value);
	}

	void simple_print(const DataContainer& container){
		std::for_each(container.begin(), container.end(), [](const auto& value){std::cout << value;});
	}

	
	void simple_insert(const int& pos, int& count, const DataType& value ,const DataContainer& data, const ChildrenContainer& children){

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


	const std::function<void(const int&, int&, const DataType&, const DataContainer&, const ChildrenContainer&)> insert = simple_insert;
	const std::function<iterator(const DataContainer&, const T&)> search = simple_search;
	const std::function<void(const DataContainer&)> print = simple_print;
};

#endif
