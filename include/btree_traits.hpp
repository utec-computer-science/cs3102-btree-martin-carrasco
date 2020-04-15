#ifndef BTREE_TRAITS_HPP
#define BTREE_TRAITS_HPP

#include <array>
#include <algorithm>
#include <functional>
#include <iostream>

template <class T>
struct Order3Trait {
  static constexpr int BTREE_ORDER = 3;
	using DataType = T;
	using DataContainer = typename std::array<T, BTREE_ORDER + 1>;
	using iterator = typename DataContainer::iterator;

	iterator simple_search(const DataContainer& container, const T& value){
			return std::find_if(container.begin(), container.end(), value);
	}

	void simple_print(const DataContainer& container){
		std::for_each(container.begin(), container.end(), [](const auto& value){std::cout << value;});
	}

	const std::function<iterator(const DataContainer&, const T&)> search = simple_search;
	const std::function<void(const DataContainer&)> print = simple_print;
};

#endif
