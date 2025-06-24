#ifndef FIXUP_CONTEXT_HPP
#define FIXUP_CONTEXT_HPP

#include "../../core/Node.hpp"
#include <functional>

template <typename CaseType, typename T> class FixupContext {
public:
  virtual ~FixupContext() = default;
  virtual CaseType getCase() const = 0;
  virtual Node<T> *useCaseAction() = 0;
  virtual Node<T> *
  fixupAction(std::function<Node<T> *(Node<T> *)> rotateLeft,
              std::function<Node<T> *(Node<T> *)>
                  rotateRight) = 0; // Always should return a leaf.
};

#endif
