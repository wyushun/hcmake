#include "naive_tensor.hpp"

#include "std_inc.hpp"

void NaiveTensor::Print() {
  ostringstream oss;

  oss << "shape: [";
  for(auto i=0u; i<shape_.size(); i++) {
      oss << shape_[i];
      if(i != shape_.size()-1) {
        oss << ", ";
      }
  }
  oss << "]\n";

  oss << "stride: [";
  for(auto i=0u; i<stride_.size(); i++) {
      oss << stride_[i];
      if(i != stride_.size()-1) {
        oss << ", ";
      }
  }
  oss << "]\n";

  cout << oss.str() << endl;
}

void NaiveTensor::PrintData() {
  ostringstream oss;

  oss << "data size: " << data_.size() << "\n";

  oss << "data: [";
  for(auto i=0u; i<data_.size(); i++) {
      oss << data_[i];
      if(i != data_.size()-1) {
        oss << ", ";
      }
  }
  oss << "]\n";

  cout << oss.str() << endl;
}
