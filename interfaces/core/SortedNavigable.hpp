#ifndef SORTED_NAVIGABLE_HPP
#define SORTED_NAVIGABLE_HPP

template <typename T> class SortedNavigable {
  virtual T minimum() = 0;
  virtual T maximum() = 0;
  virtual T successor(T value) = 0;
  virtual T predecessor(T value) = 0;
};

#endif
