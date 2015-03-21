#include "nn_alg.h"

#include <iostream>
#include <sstream>

using namespace std;

int main() {

    std::string s = R"(5
                       0 4 6 2 13
                       4 0 3 2 13
                       6 3 0 5 13
                       2 2 5 0 8
                       13 13 13 8 0
                        )";
    std::stringstream ss;
    ss << s;

    croutes::ndata_ptr<int> d = croutes::read_data<int>(ss);

    croutes::nn_alg<int> nn;
    auto a = nn.compute(d);

    for (auto b : a->bonds()) {
        std::cout << *b << std::endl;
    }
    return 0;
}