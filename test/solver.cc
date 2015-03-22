#include "nn_alg.h"
#include "prim_euler_alg.h"
#include <little_alg.h>

#include <iostream>
#include <sstream>

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

    auto d = croutes::read_data<double>(ss);

    croutes::little_alg<double> alg;
    auto a = alg.compute(d, 4);

    for (const auto& b : a->bundles()) {
        for (const auto& bond : *b) {
            std::cout << *bond << std::endl;
        }
        std::cout << "Length = " << a->total_distance(b) << std::endl << std::endl;
    }
    return 0;
}