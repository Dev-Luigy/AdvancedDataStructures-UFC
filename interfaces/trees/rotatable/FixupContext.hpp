#ifndef FIXUP_CONTEXT_HPP
#define FIXUP_CONTEXT_HPP

#include "../../core/Node.hpp"
#include <functional>

template <typename CaseType, typename T> class FixupContext {
public:
  virtual ~FixupContext() = default;
  virtual CaseType getCase() const = 0;
  virtual Node<T> *useCaseAction() = 0;
  virtual Node<T> *fixupAction() = 0; // Always should return a leaf.
};

#endif
