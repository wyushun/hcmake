#include "ir.hpp"
#include "ir_visitor.hpp"

int main(int argc, char **argv) {
  FLAGS_log_dir = "./";
  google::InitGoogleLogging(argv[0]);
  FLAGS_stderrthreshold = google::INFO;
  FLAGS_colorlogtostderr = true;
  FLAGS_logbufsecs = 0;
  FLAGS_v = 0;

  auto v1 = IntImm::make(32);
  auto v2 = IntImm::make(64);
  auto v10 = IntImm::make(64.123);
  auto v3 = BinaryExpr<Add>::make(v1.get(), v2.get());
  auto v4 = BinaryExpr<Sub>::make(v3.get(), v2.get());
  auto v5 = BinaryExpr<Sub>::make(v4.get(), v10.get());

  auto visitor = make_unique<GenIRVisitor>();

  v5->Accept(visitor.get());
  cout << endl;

  cout << Expr::TypeName.at(v5->GetValueType()) << endl;

  google::ShutdownGoogleLogging();
  return 0;
}
