#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "DataStructure.hpp"
#include <vector>
template <typename T> class PriorityQueue : public DataStructure<T> {
public:
  virtual ~PriorityQueue() {};
  std::vector<T> m_vec{T()};
};

#endif
