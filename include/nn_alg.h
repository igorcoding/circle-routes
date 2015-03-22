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
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node);
    };


    template <typename T>
    nn_alg<T>::nn_alg() {

    }

    template <typename T> inline
    answer_ptr<T> nn_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node) {
        if (data == nullptr) {
            return nullptr;
        }

        auto size = data->nodes_count();

        std::vector<int32_t> visited(size, false);

        auto ans = answer<T>::init();

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
                ans->add_bond(min);
                visited[current_node] = true;
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
