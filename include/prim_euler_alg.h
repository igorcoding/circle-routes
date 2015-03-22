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

    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, uint32_t first_node);
    private:

    };

    template <typename T>
    prim_euler_alg<T>::prim_euler_alg() {

    }

    template <typename T> inline
    answer_ptr<T> prim_euler_alg<T>::_compute(ndata_ptr<T> data, uint32_t first_node) {
        auto size = data->nodes_count();
        typedef const net_bond<T>* bond_t;
        std::vector<bond_t> min_span_tree;

        std::vector<uint32_t> nodes(size);
        for (uint32_t i = 0; i < size; ++i) {
            nodes[i] = i;
        }

        std::vector<T*> d(size, nullptr);
        std::vector<uint32_t*> p(size, nullptr);
        std::vector<bool> active(size, true);

        auto zero = 0;
        d[first_node] = &zero;

        auto less = [&d](uint32_t lhs, uint32_t rhs) {
            if (d[lhs] == nullptr && d[rhs] == nullptr) {
                return false;
            }
            if (d[lhs] == nullptr) {
                return false;
            }
            if (d[rhs] == nullptr) {
                return true;
            }
            return *d[lhs] < *d[rhs];
        };

        for (uint32_t i = 0; i < size; ++i) {
            uint32_t* v = nullptr;
            for (uint32_t j = 0; j < size; ++j) {
                if (active[j] && (v == nullptr || less(j, *v))) {
                    v = &nodes[j];
                }
            }

            if (v == nullptr) {
                std::cout << "null\n";
            } else {
                active[*v] = false;
                if (p[*v] != nullptr) {
//                    std::cout << *v << " " << *p[*v] << std::endl;
                    min_span_tree.push_back(&data->at(*p[*v], *v));
                }

                for (uint32_t u = 0; u < size; ++u) {
                    auto n = data->at(*v, u);

                    if (d[u] == nullptr || n.distance() < *d[u]) {
                        delete d[u];
                        delete p[u];
                        d[u] = new T(n.distance());
                        p[u] = new uint32_t(*v);
                    }
                }
            }
        }


        std::stack<uint32_t> back_stack;
        std::vector<uint32_t> result;

        result.push_back(min_span_tree[0]->from());
        result.push_back(min_span_tree[0]->to());

        back_stack.push(min_span_tree[0]->from());
        back_stack.push(min_span_tree[0]->to());


        while (!back_stack.empty()) {
            auto back = result.back();
            auto it = std::find_if(min_span_tree.begin(), min_span_tree.end(), [&back](bond_t& b) {
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

        active.clear();
        active.resize(size, false);

        std::vector<uint32_t> route;

        for (size_t i = 0; i < result.size() - 1; ++i) {
            auto r = result[i];

            if (!active[r]) {
                route.push_back(r);
                active[r] = true;
            }
        }
        route.push_back(result[result.size() - 1]);


        answer_ptr<T> ans = std::make_shared<answer<T>>();
        for (size_t i = 0; i < route.size() - 1; ++i) {
            ans->add_bond(&data->at(route[i], route[i+1]));
        }


        return ans;
    }
}

#endif //_CIRCLE_ROUTES_PRIM_EULER_ALG_H_
