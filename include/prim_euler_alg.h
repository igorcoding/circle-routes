#ifndef _CIRCLE_ROUTES_PRIM_EULER_ALG_H_
#define _CIRCLE_ROUTES_PRIM_EULER_ALG_H_

#include "algorithm.h"

#include <vector>
#include <queue>
#include <utility>

namespace croutes {

    namespace prim_euler_alg_details {
//        class pq_comparison
//        {
//        public:
//            mycomparison(const bool& revparam=false)
//            {reverse=revparam;}
//            bool operator() (const int& lhs, const int&rhs) const
//            {
//                if (reverse) return (lhs>rhs);
//                else return (lhs<rhs);
//            }
//
//        private:
//
//        };
    }

    template <typename T>
    class prim_euler_alg : public algorithm<T> {
    public:
        prim_euler_alg();

    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data);
    private:

    };

    template <typename T>
    prim_euler_alg<T>::prim_euler_alg() {

    }

    template <typename T> inline
    answer_ptr<T> prim_euler_alg<T>::_compute(ndata_ptr<T> data) {
        auto size = data->nodes_count();
        std::vector<const net_bond<T>*> min_span_tree;

        std::vector<uint32_t> nodes(size);
        for (uint32_t i = 0; i < size; ++i) {
            nodes[i] = i;
        }

        std::vector<T*> d(size, nullptr);
        std::vector<uint32_t*> p(size, nullptr);
        std::vector<bool> active(size);

        auto zero = 0;
        d[0] = &zero;


        typedef std::pair<uint32_t, const T*> pq_item_t;

        auto comp = [](const pq_item_t& lhs, const pq_item_t& rhs) {
            if (lhs.second == nullptr && rhs.second == nullptr) {
                return true;
            }

            if (lhs.second == nullptr) {
                return true;
            }
            if (rhs.second == nullptr) {
                return false;
            }

            return *lhs.second > *rhs.second;
        };

//        typedef std::priority_queue<pq_item_t, std::vector<pq_item_t>, decltype(comp)> pq_t;
//        pq_t pq(comp);

//        for (uint32_t i = 0; i < size; ++i) {
//            pq.push(std::make_pair(i, d[i]));
//            active[i] = true;
//        }

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
                    std::cout << *v << " " << *p[*v] << std::endl;
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



        return std::shared_ptr<answer<T>>();
    }
}

#endif //_CIRCLE_ROUTES_PRIM_EULER_ALG_H_
