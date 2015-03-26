#include "nn_alg.h"
#include "prim_euler_alg.h"
#include "little_alg.h"
#include "nearest_alg.h"

#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>

template <typename T>
void expirement(croutes::ndata_ptr<T> data) {
    croutes::algorithm<T>* algs[] = {
//            new croutes::nn_alg<T>(),
//            new croutes::prim_euler_alg<T>(),
            new croutes::little_alg<T>(),
//            new croutes::nearest_alg<T>()
    };

    for (auto& alg : algs) {
        auto a = alg->compute(data);
        auto minimum = a->find_min_distance();

        std::cout << "=== " << alg->name() << " ===" << std::endl;
        if (!minimum.first) {
            std::cout << "error";
            delete alg;
            continue;
        }
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

int main(int argc, char** argv) {

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    std::string filename;
    desc.add_options()
            ("help,h", "Produce help message")
            ("filename,f", po::value<std::string>(&filename), "Filename")
            ("console,c", "Interactive console")
            ("test,t", "Just some testing data")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    croutes::ndata_ptr<double> d;
    if (vm.count("filename") && !vm.count("console") && !vm.count("test")) {
        d = croutes::read_file<double>(filename);
    } else if (vm.count("console") && !vm.count("test")) {
        d = croutes::read_console<double>();
    } else if (vm.count("test")) {
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

        std::string s3 = R"(
6
0 84 12 48 32 64
84 0 14 18 20 22
12 14 0 24 28 16
48 18 24 0 14 10
32 20 28 14 0 8
64 22 16 10 8 0
)";
        std::stringstream ss;
        ss << s;
        d = croutes::read_data<double>(ss);
    } else {
        std::cout << desc << std::endl;
        return 1;
    }
    std::cout << std::endl;

    expirement(d);
    return 0;
}
