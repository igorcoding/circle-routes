#ifndef _CIRCLE_ROUTES_NN_ALG_H_
#define _CIRCLE_ROUTES_NN_ALG_H_

#include "algorithm.h"

#include <queue>
#include <algorithm>

namespace croutes {
    template <typename T>
    class nn_alg : public algorithm<T> {
    public:
        nn_alg();

        virtual const std::string& short_name() override { return _short_name; }
        virtual const std::string& name() override { return _name; }
        virtual const std::string& russian_name() override { return _russian_name; }
    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node) override;

    private:
        std::string _short_name;
        std::string _name;
        std::string _russian_name;
    };


    template <typename T>
    nn_alg<T>::nn_alg()
        : _short_name("nn_alg"),
          _name("Nearest Neighbor algorithm"),
          _russian_name("Алгоритм \"Иди в ближний\"") {

    }

    template <typename T> inline
    answer_ptr<T> nn_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node_) {
        auto size = data->nodes_count();

        auto ans = answer<T>::init();

        for (int32_t first_node = 0; first_node < size; ++first_node) {
            auto bundle = ans->create_bundle();
            std::vector<int32_t> visited(size, false);

            int32_t current_node = first_node;
            while (true) {
                const net_bond<T>* min = nullptr;
                for (int32_t j = 0; j < size; ++j) {
                    if (j != current_node) {
                        auto at_j = &data->at(current_node, j);
                        if ((min == nullptr || at_j->distance() < min->distance()) && !visited[j]) {
                            min = at_j;
                        }
                    }
                }

                if (min != nullptr) {
                    ans->add_bond(bundle, min);
                    visited[current_node] = true;
                    current_node = min->to();
                } else {
                    ans->add_bond(bundle, &data->at(current_node, first_node));
                    break;
                }
            }
        }

        return ans;
    }
}

#endif //_CIRCLE_ROUTES_NN_ALG_H_
