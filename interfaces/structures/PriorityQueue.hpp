#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "../core/DataStructure.hpp"
#include "../core/Node.hpp"
template <typename T> class PriorityQueue : public DataStructure<T> {
public:
  virtual ~PriorityQueue() {};
  virtual Node<T> *top() = 0;
};

#endif
