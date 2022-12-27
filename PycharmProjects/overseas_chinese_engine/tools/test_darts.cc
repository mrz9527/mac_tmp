#include <iostream>
#include <string>
#include <algorithm>
#include <darts.h>

int main (int argc, char **argv)
{
  using namespace std;

  Darts::DoubleArray da;
  if (da.open("some_file") == -1) return -1;

  Darts::DoubleArray::result_type  r [1024];
  Darts::DoubleArray::key_type     buf [1024];

  while (cin.getline (buf, 1024)) {
    size_t result = da.commonPrefixSearch(buf, r, 1024);
    if (result == 0) {
       cout << buf << ": not found" << endl;
    } else {
       cout << buf << ": found, num=" << result << " ";
       copy (r, r + result, ostream_iterator<Darts::DoubleArray::result_type>(cout, " "));
       cout << endl;
    }
  }

  return 0;
}
