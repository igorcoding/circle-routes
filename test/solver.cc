#include "nn_alg.h"
#include "prim_euler_alg.h"
#include <little_alg.h>

#include <iostream>
#include <sstream>

template <typename T>
void expirement(croutes::ndata_ptr<T> data) {
    croutes::algorithm<T>* algs[] = {
            new croutes::nn_alg<T>(),
            new croutes::prim_euler_alg<T>(),
            new croutes::little_alg<T>()
    };

    for (auto& alg : algs) {
        auto a = alg->compute(data);
        auto minimum = a->find_min_distance();

        std::cout << "=== " << alg->name() << " ===" << std::endl;
        for (const auto& bond : *minimum.first) {
            std::cout << *bond << std::endl;
        }
        std::cout << "Length = " << minimum.second << std::endl << std::endl;

        std::cout << "--------" << std::endl;

        for (const auto& b : a->bundles()) {
            for (const auto& bond : *b) {
                std::cout << *bond << std::endl;
            }
            std::cout << "Length = " << a->total_distance(b) << std::endl << std::endl;
        }

        delete alg;
        alg = nullptr;
    }
}

int main() {

    std::string s = R"(5
                       0 4 6 2 13
                       4 0 3 2 13
                       6 3 0 5 13
                       2 2 5 0 8
                       13 13 13 8 0
                        )";
    std::string s2 = R"(6
            0 10 2 5 4 10
            10 0 7 9 11 8
            2 7 0 10 7 6
            5 9 10 0 8 3
            4 11 7 8 0 2
            10 8 6 3 2 0
                        )";
    std::stringstream ss;
    ss << s2;

    auto d = croutes::read_data<double>(ss);

    expirement(d);

//    croutes::little_alg<double> alg;
//    auto a = alg.compute(d, 4);
//
//    for (const auto& b : a->bundles()) {
//        for (const auto& bond : *b) {
//            std::cout << *bond << std::endl;
//        }
//        std::cout << "Length = " << a->total_distance(b) << std::endl << std::endl;
//    }
    return 0;
}
