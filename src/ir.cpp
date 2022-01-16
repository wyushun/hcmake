#include "ir.hpp"

// clang-format off
const map<ExprType, string> Expr::TypeName = {
    {ExprType::IntImm, "IntImm"},
    {ExprType::FloatImm, "FloatImm"},
    {ExprType::StringImm, "StringImm"},
    {ExprType::Add, "Add"},
    {ExprType::Sub, "Sub"},
    {ExprType::Mul, "Mul"},
    {ExprType::Div, "Div"}};

const map<ExprType, string> Expr::OperatorName = {
    {ExprType::IntImm, ""},
    {ExprType::FloatImm, ""},
    {ExprType::StringImm, ""},
    {ExprType::Add, "+"},
    {ExprType::Sub, "-"},
    {ExprType::Mul, "*"},
    {ExprType::Div, "/"}};
// clang-format on

unique_ptr<IntImm> IntImm::make(int64_t value) {
  return make_unique<IntImm>(value);
}

unique_ptr<FloatImm> FloatImm::make(double value) {
  return make_unique<FloatImm>(value);
}
