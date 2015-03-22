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

    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, uint32_t first_node);
    };


    template <typename T>
    nn_alg<T>::nn_alg() {

    }

    template <typename T> inline
    answer_ptr<T> nn_alg<T>::_compute(ndata_ptr<T> data, uint32_t first_node) {
        if (data == nullptr) {
            return nullptr;
        }

        std::set<uint32_t> visited;
        auto is_visited = [&visited](uint32_t node) {
            return visited.find(node) != visited.end();
        };

        auto size = data->nodes_count();

        answer_ptr<T> ans = std::make_shared<answer<T>>();

        bool last = false;
        uint32_t current_node = first_node;
        while (true) {
            const net_bond<T>* min = nullptr;
            for (uint32_t j = 0; j < size; ++j) {
                if (j != current_node) {
                    auto at_j = &data->at(current_node, j);
                    if ((min == nullptr || at_j->distance() < min->distance()) && !is_visited(j)) {
                        min = at_j;
                    }
                }
            }

            if (min != nullptr) {
                ans->add_bond(min);
                visited.insert(current_node);
                current_node = min->to();
            } else {
                ans->add_bond(&data->at(current_node, first_node));
                break;
            }
        }

        return ans;
    }


}

#endif //_CIRCLE_ROUTES_NN_ALG_H_
