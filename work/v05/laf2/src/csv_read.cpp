#include "csvreader.h"
#include "eventhandler_columns.h"

#include <vector>
#include <Rcpp.h>
using namespace Rcpp;


// [[Rcpp::export]]
List csv_read_cpp(std::string filename, std::string column_types) {
  // Set up event handler
  EventHandlerColumns handler;
  std::vector<Column*> columns_;
  for (auto p = column_types.begin(); p != column_types.end(); ++p) {
    if (*p == 'i') {
      ArrayColumn<int>* col = new ArrayColumn<int>();
      columns_.push_back(col);
      handler.add_column(col);
    } else if (*p == 'd') {
      ArrayColumn<double>* col = new ArrayColumn<double>();
      columns_.push_back(col);
      handler.add_column(col);
    }
  }
  // Open and read file
  CSVReader<EventHandlerColumns> reader(filename, handler);
  reader.parse();
  // Copy data to R-structures and cleanup memory
  List res(columns_.size());
  for (std::size_t i = 0; i < columns_.size(); ++i) {
    res[i] = columns_[i]->sexp();
    delete columns_[i];
    columns_[i] = 0;
  }
  return res;
}
