#ifndef _CIRCLE_ROUTES_PRIM_EULER_ALG_H_
#define _CIRCLE_ROUTES_PRIM_EULER_ALG_H_

#include "algorithm.h"

#include <vector>
#include <queue>
#include <utility>
#include <stack>
#include <unordered_map>
#include <algorithm>

namespace croutes {

    template <typename T>
    class prim_euler_alg : public algorithm<T> {
    public:
        prim_euler_alg();

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
    prim_euler_alg<T>::prim_euler_alg()
            : _short_name("prim_euler_alg"),
              _name("Prim-Euler algorithm"),
              _russian_name("Алгоритм Прима-Эйлера") {

    }

    template <typename T> inline
    answer_ptr<T> prim_euler_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node_) {
        auto size = data->nodes_count();
        typedef const net_bond<T>* bond_t;

        auto ans = answer<T>::init();

        for (int32_t first_node = 0; first_node < size; ++first_node) {
            auto bundle = ans->create_bundle();

            std::vector<bond_t> min_span_tree;

            std::vector<T> d(size, data->inf());
            std::vector<int32_t> p(size, -1);

            std::vector<bool> active(size, true);

            d[first_node] = T(0);

            auto less = [ &d ](int32_t lhs, int32_t rhs) {
                return d[lhs] < d[rhs];
            };

            for (int32_t i = 0; i < size; ++i) {
                int32_t v = -1;
                for (int32_t j = 0; j < size; ++j) {
                    if (active[j] && (v == -1 || less(j, v))) {
                        v = j;
                    }
                }

                if (v == -1) {
                    std::cout << "null\n";
                } else {
                    active[v] = false;
                    if (p[v] != -1) {
                        min_span_tree.push_back(&data->at(p[v], v));
                    }

                    for (int32_t u = 0; u < size; ++u) {
                        auto n = data->at(v, u);

                        if (n.distance() < d[u]) {
                            d[u] = n.distance();

                            p[u] = v;
                        }
                    }
                }
            }


            std::stack<int32_t> back_stack;
            std::vector<int32_t> result;

            result.push_back(min_span_tree[0]->from());
            result.push_back(min_span_tree[0]->to());

            back_stack.push(min_span_tree[0]->from());
            back_stack.push(min_span_tree[0]->to());

            min_span_tree.erase(min_span_tree.begin());


            while (!back_stack.empty()) {
                auto back = result.back();
                auto it = std::find_if(min_span_tree.begin(), min_span_tree.end(), [ &back ](bond_t& b) {
                    return b->from() == back;
                });

                if (it == min_span_tree.end()) {
                    back_stack.pop();
                    if (!back_stack.empty()) {
                        result.push_back(back_stack.top());
                    }
                } else {
                    result.push_back((*it)->to());
                    back_stack.push((*it)->to());
                    min_span_tree.erase(it);
                }
            }

            int32_t lhs = result[0];
            active[lhs] = true;
            for (size_t i = 1; i < result.size() - 1; ++i) {
                auto r = result[i];

                if (!active[r]) {
                    ans->add_bond(bundle, &data->at(lhs, r));
                    lhs = r;
                    active[r] = true;
                }
            }
            ans->add_bond(bundle, &data->at(lhs, result.back()));
        }

//        ans->unique();
        return ans;
    }
}

#endif //_CIRCLE_ROUTES_PRIM_EULER_ALG_H_
