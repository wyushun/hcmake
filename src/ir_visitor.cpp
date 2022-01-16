#include "ir_visitor.hpp"

#include "ir.hpp"

void PrintVisitor::visit(IntImm* e) { cout << e->GetValue(); }

void PrintVisitor::visit(FloatImm* e) { cout << e->GetValue(); }

void PrintVisitor::visit(Add* e) {
  cout << "(";
  e->GetLHS()->Accept(this);
  cout << "+";
  e->GetRHS()->Accept(this);
  cout << ")";
}

void PrintVisitor::visit(Sub* e) {
  cout << "(";
  e->GetLHS()->Accept(this);
  cout << "-";
  e->GetRHS()->Accept(this);
  cout << ")";
}

void PrintVisitor::visit(Mul* e) {
  cout << "(";
  e->GetLHS()->Accept(this);
  cout << "*";
  e->GetRHS()->Accept(this);
  cout << ")";
}

void PrintVisitor::visit(Div* e) {
  cout << "(";
  e->GetLHS()->Accept(this);
  cout << "/";
  e->GetRHS()->Accept(this);
  cout << ")";
}

void GenIRVisitor::visit(IntImm* e) {
  if (labels_.find(e) == labels_.end()) {
    cout << "%" << cnt_ << " = " << e->GetValue() << endl;
    labels_[e] = cnt_++;
  }
}

void GenIRVisitor::visit(FloatImm* e) {
  if (labels_.find(e) == labels_.end()) {
    cout << "%" << cnt_ << " = " << e->GetValue() << endl;
    labels_[e] = cnt_++;
  }
}

void GenIRVisitor::visit(Add* e) { VisitBinary<Add>(e); }
void GenIRVisitor::visit(Sub* e) { VisitBinary<Sub>(e); }
void GenIRVisitor::visit(Mul* e) { VisitBinary<Mul>(e); }
void GenIRVisitor::visit(Div* e) { VisitBinary<Div>(e); }
