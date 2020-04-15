#ifndef BTREE_TRAITS_HPP
#define BTREE_TRAITS_HPP

struct Order3Trait {
  static constexpr int BTREE_ORDER = 3;
  using DataType = int;
};

#endif
