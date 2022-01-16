#pragma once

#include "std_inc.hpp"

class Expr;
class IntImm;
class FloatImm;
class Add;
class Sub;
class Mul;
class Div;

class Visitor {
 public:
  Visitor() = default;
  virtual ~Visitor() = default;

  virtual void visit(IntImm*) = 0;
  virtual void visit(FloatImm*) = 0;
  virtual void visit(Add*) = 0;
  virtual void visit(Sub*) = 0;
  virtual void visit(Mul*) = 0;
  virtual void visit(Div*) = 0;
};

class PrintVisitor : public Visitor {
 public:
  PrintVisitor() = default;
  ~PrintVisitor() = default;

 public:
  virtual void visit(IntImm*) override final;
  virtual void visit(FloatImm*) override final;
  virtual void visit(Add*) override final;
  virtual void visit(Sub*) override final;
  virtual void visit(Mul*) override final;
  virtual void visit(Div*) override final;
};

class GenIRVisitor : public Visitor {
 private:
  uint32_t cnt_;
  map<void*, uint32_t> labels_;

 public:
  GenIRVisitor() : cnt_(0) {}
  ~GenIRVisitor() = default;

 public:
  virtual void visit(IntImm*) override final;
  virtual void visit(FloatImm*) override final;
  virtual void visit(Add*) override final;
  virtual void visit(Sub*) override final;
  virtual void visit(Mul*) override final;
  virtual void visit(Div*) override final;

 private:
  template <typename T>
  void VisitBinary(T* e) {
    if (labels_.find(e) != labels_.end()) {
      return;
    }

    e->GetLHS()->Accept(this);
    e->GetRHS()->Accept(this);

    std::ostringstream oss;
    oss << "%" << cnt_ << " = %" << labels_[e->GetLHS()] << " "
        << e->GetOperatorName() << " %" << labels_[e->GetRHS()];
    cout << oss.str() << endl;
    labels_[e] = cnt_++;
  }
};
