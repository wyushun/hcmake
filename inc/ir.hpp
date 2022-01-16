#pragma once

#include "ir_visitor.hpp"
#include "std_inc.hpp"

enum class ExprType {
  IntImm,
  FloatImm,
  StringImm,
  Add,
  Sub,
  Mul,
  Div,
};

class Expr {
 protected:
  ExprType type_;
  string type_name_;
  string operator_name_;

 public:
  const static map<ExprType, string> TypeName;
  const static map<ExprType, string> OperatorName;

 public:
  Expr() = delete;
  Expr(ExprType type) : type_(type), type_name_(TypeName.at(type)) {}
  virtual ~Expr() = default;

  ExprType GetType() const { return type_; }
  string GetTypeName() const { return type_name_; }
  string GetOperatorName() const { return OperatorName.at(type_); }

  virtual bool IsLeaf() const = 0;
  virtual tuple<bool, ExprType, ExprType> IsTypeMatched() const = 0;
  virtual ExprType GetValueType() const = 0;

  virtual void Accept(Visitor* v) = 0;
};

class IntImm : public Expr {
 private:
  int64_t value_;

 public:
  IntImm(int64_t value) : Expr(ExprType::IntImm), value_(value) {}
  ~IntImm() = default;

  int64_t GetValue() const { return value_; }
  virtual bool IsLeaf() const override final { return true; }
  virtual tuple<bool, ExprType, ExprType> IsTypeMatched() const override final {
    return std::make_tuple(true, GetType(), GetType());
  }
  virtual ExprType GetValueType() const override final { return GetType(); }
  virtual void Accept(Visitor* v) override final { v->visit(this); }

 public:
  static unique_ptr<IntImm> make(int64_t value);
};

class FloatImm : public Expr {
 private:
  double value_;

 public:
  FloatImm(double value) : Expr(ExprType::FloatImm), value_(value) {}
  ~FloatImm() = default;

  double GetValue() const { return value_; }
  virtual bool IsLeaf() const override final { return true; }
  virtual tuple<bool, ExprType, ExprType> IsTypeMatched() const override final {
    return std::make_tuple(true, GetType(), GetType());
  }
  virtual ExprType GetValueType() const override final { return GetType(); }
  virtual void Accept(Visitor* v) override final { v->visit(this); }

 public:
  static unique_ptr<FloatImm> make(double value);
};

template <typename T>
class BinaryExpr : public Expr {
 protected:
  Expr* lhs_;
  Expr* rhs_;

 public:
  BinaryExpr(ExprType type, Expr* lhs, Expr* rhs)
      : Expr(type), lhs_(lhs), rhs_(rhs) {}
  ~BinaryExpr() = default;

  Expr* GetLHS() const { return lhs_; }
  Expr* GetRHS() const { return rhs_; }
  virtual bool IsLeaf() const override final { return false; }
  virtual tuple<bool, ExprType, ExprType> IsTypeMatched() const override final;
  virtual ExprType GetValueType() const override final;
  virtual void Accept(Visitor* v) override final {
    v->visit(static_cast<T*>(this));
  }

 public:
  static unique_ptr<T> make(Expr* lhs, Expr* rhs);
};

class Add : public BinaryExpr<Add> {
 public:
  Add(Expr* lhs, Expr* rhs) : BinaryExpr(ExprType::Add, lhs, rhs) {}
  ~Add() = default;
};

class Sub : public BinaryExpr<Sub> {
 public:
  Sub(Expr* lhs, Expr* rhs) : BinaryExpr(ExprType::Sub, lhs, rhs) {}
  ~Sub() = default;
};

class Mul : public BinaryExpr<Mul> {
 public:
  Mul(Expr* lhs, Expr* rhs) : BinaryExpr(ExprType::Mul, lhs, rhs) {}
  ~Mul() = default;
};

class Div : public BinaryExpr<Div> {
 public:
  Div(Expr* lhs, Expr* rhs) : BinaryExpr(ExprType::Div, lhs, rhs) {}
  ~Div() = default;
};

template <typename T>
unique_ptr<T> BinaryExpr<T>::make(Expr* lhs, Expr* rhs) {
  return make_unique<T>(lhs, rhs);
}

template <typename T>
tuple<bool, ExprType, ExprType> BinaryExpr<T>::IsTypeMatched() const {
  if (GetLHS()->IsLeaf() && GetRHS()->IsLeaf()) {
    auto lhs_type = GetLHS()->GetType();
    auto rhs_type = GetRHS()->GetType();
    if (lhs_type != rhs_type) {
      LOG(INFO) << "err: " << TypeName.at(lhs_type) << " mismatch "
                << TypeName.at(rhs_type);
      return std::make_tuple(false, lhs_type, rhs_type);
    } else {
      return std::make_tuple(true, lhs_type, rhs_type);
    }
  } else if (GetLHS()->IsLeaf() && !GetRHS()->IsLeaf()) {
    auto rhs_match_rlt = GetRHS()->IsTypeMatched();
    if (!std::get<0>(rhs_match_rlt)) {
      return std::make_tuple(false, std::get<1>(rhs_match_rlt),
                             std::get<2>(rhs_match_rlt));
    } else {
      auto lhs_type = GetLHS()->GetType();
      if (lhs_type != std::get<1>(rhs_match_rlt)) {
        LOG(INFO) << "err: " << TypeName.at(lhs_type) << " mismatch "
                  << TypeName.at(std::get<1>(rhs_match_rlt));
        return std::make_tuple(false, lhs_type, std::get<1>(rhs_match_rlt));
      } else {
        return std::make_tuple(true, lhs_type, std::get<1>(rhs_match_rlt));
      }
    }
  } else if (!GetLHS()->IsLeaf() && GetRHS()->IsLeaf()) {
    auto lhs_match_rlt = GetLHS()->IsTypeMatched();
    if (!std::get<0>(lhs_match_rlt)) {
      return std::make_tuple(false, std::get<1>(lhs_match_rlt),
                             std::get<2>(lhs_match_rlt));
    } else {
      auto rhs_type = GetRHS()->GetType();
      if (std::get<1>(lhs_match_rlt) != rhs_type) {
        LOG(INFO) << "err: " << TypeName.at(rhs_type) << " mismatch "
                  << TypeName.at(std::get<1>(lhs_match_rlt));
        return std::make_tuple(false, std::get<1>(lhs_match_rlt), rhs_type);
      } else {
        return std::make_tuple(true, std::get<1>(lhs_match_rlt), rhs_type);
      }
    }
  } else {
    auto lhs_match_rlt = GetLHS()->IsTypeMatched();
    auto rhs_match_rlt = GetRHS()->IsTypeMatched();
    if (!std::get<0>(lhs_match_rlt)) {
      return std::make_tuple(false, std::get<1>(lhs_match_rlt),
                             std::get<2>(lhs_match_rlt));
    } else if (!std::get<0>(rhs_match_rlt)) {
      return std::make_tuple(false, std::get<1>(rhs_match_rlt),
                             std::get<2>(rhs_match_rlt));
    } else {
      if (std::get<1>(lhs_match_rlt) != std::get<1>(rhs_match_rlt)) {
        LOG(INFO) << "err: " << TypeName.at(std::get<1>(lhs_match_rlt))
                  << " mismatch " << TypeName.at(std::get<1>(rhs_match_rlt));
        return std::make_tuple(false, std::get<1>(lhs_match_rlt),
                               std::get<1>(rhs_match_rlt));
      } else {
        return std::make_tuple(true, std::get<1>(lhs_match_rlt),
                               std::get<1>(rhs_match_rlt));
      }
    }
  }
}

template <typename T>
ExprType BinaryExpr<T>::GetValueType() const {
  auto match_rlt = IsTypeMatched();
  CHECK(std::get<0>(match_rlt));
  return std::get<1>(match_rlt);
}
