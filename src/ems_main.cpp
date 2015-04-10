#include <iostream>
#include <string>

#include "../include/nn_alg.h"
#include "../include/prim_euler_alg.h"
#include "../include/little_alg.h"
#include "../include/nearest_with_ret_alg.h"
#include "../include/nearest_alg.h"
#include "../include/ndata.h"

#include "../libs/picojson/picojson.h"

using namespace croutes;

namespace croutes {

    template <typename T>
    ndata_ptr<T> read_json(const std::string& json) {
        using namespace picojson;

        value v;
        std::string err = parse(v, json);
        if (!err.empty()) {
           std::cerr << err << std::endl;
           return nullptr;
        }

        const value::object& obj = v.get<object>();

        size_t size = (size_t) obj.at("size").get<double>();
        array data = obj.at("arr").get<array>();

        auto d = std::make_shared<ndata<T>>(size);
        for (size_t i = 0; i < data.size(); ++i) {
            const array& col = data.at(i).get<array>();

            for (size_t j = 0; j < col.size(); ++j) {
                T value = (T) col.at(j).get<double>();
                d->add_node((int32_t) i, (int32_t) j, value);
            }
        }

        return d;
    }
}

extern "C" int sum (int a, int b) {
    return a + b;
}

//yes, this is global variable
//just because...
std::string calculate_result;
extern "C" const char* calculate (char* data) {
    std::cout << data << std::endl;

    using namespace picojson;

    ndata_ptr<double> ndata = read_json(data);

    if (ndata == nullptr)
        return "";

    croutes::algorithm<double>* algs[] = {
        new croutes::nn_alg<double>(),
        new croutes::prim_euler_alg<double>(),
        new croutes::little_alg<double>(),
        new croutes::nearest_alg<double>(),
        new croutes::nearest_with_ret_alg<double>(),
    };

    object res;
    for (auto& alg: algs) {
        auto a = alg->compute(ndata);
        object alg_obj;
        array results_arr;

        for (const auto& b: a->bundles()) {
            array json_bundle;
            for (const auto& bond : *b) {
                object json_bond;
                json_bond["from"] = value((double) bond->from());
                json_bond["to"] = value((double) bond->to());
                json_bond["distance"] = value((double) bond->distance());

                json_bundle.push_back(value(json_bond));
            }
            results_arr.push_back(value(json_bundle));
        }

        alg_obj["name"] = value(alg->russian_name());
        alg_obj["results"] = value(results_arr);
        res[alg->short_name()] = value(alg_obj);

        delete alg;
        alg = nullptr;
    }

    calculate_result = value(res).serialize();
    return calculate_result.c_str();
}

int main () {
    std::cout << "Hello, JS!" << std::endl;
    std::cout << calculate("{\"size\": 2, \"arr\": [[1, 2], [2, 1]]}") << std::endl;
}
