#ifndef FIXUP_CONTEXT_HPP
#define FIXUP_CONTEXT_HPP

#include "../../core/Node.hpp"

template <typename CaseType, typename T> class FixupContext {
public:
  virtual ~FixupContext() = default;
  virtual CaseType getCase() const = 0;
  virtual Node<T> *
  useCaseAction(Node<T> *m_root) = 0; // Always should return a leaf.
  virtual Node<T> *
  fixupAction(Node<T> *m_root) = 0; // Always should return a leaf.
};

#endif
