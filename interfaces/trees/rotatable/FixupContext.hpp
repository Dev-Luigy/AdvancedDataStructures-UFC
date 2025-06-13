#ifndef FIXUP_CONTEXT_HPP
#define FIXUP_CONTEXT_HPP

template <typename CaseType> class FixupContext {
public:
  virtual ~FixupContext() = default;
  virtual CaseType getCase() const = 0;
};

#endif
