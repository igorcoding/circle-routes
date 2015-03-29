#ifndef _CIRCLE_ROUTES_NEAREST_WITH_RET_ALG_H_
#define _CIRCLE_ROUTES_NEAREST_WITH_RET_ALG_H_


#include "algorithm.h"

#include <queue>
#include <algorithm>
#include <assert.h>

namespace croutes {
    template <typename T>
    class nearest_with_ret_alg : public algorithm<T> {
    public:
        nearest_with_ret_alg();

        virtual const std::string& short_name() override { return _short_name; }
        virtual const std::string& name() override { return _name; }
        virtual const std::string& russian_name() override { return _russian_name; }
    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node) override;
        void worker(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, const std::vector<net_bond_ptr<T>>& queue, int32_t group_begin, int32_t current, std::vector<int32_t> edges, std::vector<bool> visited, bool recursive);
        bool try_to_add(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, net_bond_ptr<T> bond, std::vector<int32_t>& edges, std::vector<bool>& visited);

    private:
        std::string _short_name;
        std::string _name;
        std::string _russian_name;
    };


    template <typename T>
    nearest_with_ret_alg<T>::nearest_with_ret_alg()
            : _short_name("nearest_with_ret_alg"),
              _name("To the nearset algorithm (with return)"),
              _russian_name("Алгоритм \"Иди в ближний\" (с возвратом)") {

    }

    template <typename T> inline
    answer_ptr<T> nearest_with_ret_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node_) {
        auto size = data->nodes_count();

        auto ans = answer<T>::init();

        auto bundle = ans->create_bundle();
        std::vector<bool> visited(size, false);
        std::vector<int32_t> edges(2, -1);

        std::vector<net_bond_ptr<T>> queue;
        queue.reserve(size * (size - 1) / 2);

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = i+1; j < size; ++j) {
                if (j != i) {
                    queue.push_back(&data->at(i, j));
                }
            }
        }
        std::sort(queue.begin(), queue.end(), [](const net_bond_ptr<T>& lhs, const net_bond_ptr<T>& rhs) {
            if (lhs->same(*rhs)) {
                return true;
            }
            return lhs->distance() < rhs->distance();
        });

        worker(data, ans, bundle, queue, 0, -1, edges, visited, false);
//        if (ans->bundles().size() > 1) {
//
//        }

        for (auto& b : ans->bundles()) {
            ans->add_bond(b, &data->at(b->back()->to(), b->front()->from()));
        }
//        ans->unique();

        return ans;
    }

    template <typename T> inline
    void nearest_with_ret_alg<T>::worker(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, const std::vector<net_bond_ptr<T>>& queue, int32_t group_begin, int32_t current, std::vector<int32_t> edges, std::vector<bool> visited, bool recursive) {
        net_bond_ptr<T> prev_bond = nullptr;
        T* prev_distance = nullptr;
        auto current_group_begin = group_begin;


//        if (current >= 0) {
//            if (try_to_add(data, ans, bundle, queue[current], edges, visited)) {
//                prev_bond = queue[current];
//            }
//            prev_distance = &queue[current]->distance();
//        }

//        auto start = group_begin == current ? group_begin + 1 : group_begin;
//        assert (start >= 0);

        auto s = (int32_t) queue.size();

        std::vector<bool> queue_visited(queue.size(), false);

        auto is_queue_visited_empty = [&queue_visited, &data]() {
            return std::count(queue_visited.begin(), queue_visited.end(), true) == data->nodes_count() - 1;
        };

        while (!is_queue_visited_empty()) {
            for (auto j = 0; j < s; ++j) {
                if (!queue_visited[j] && try_to_add(data, ans, bundle, queue[j], edges, visited)) {
//                    prev_bond = queue[j];
                    queue_visited[j] = true;
                    j = 0;
                    break;
                }

            }
        }

//        for (auto j = 0; j < s; ++j) {
//            if (try_to_add(data, ans, bundle, queue[j], edges, visited)) {
//                prev_bond = queue[j];
//            }
//            if (j == current) {
//                continue;
//            }
//
//            if (!prev_bond || !queue[j]->same(*prev_bond)) {
//                if (prev_distance == nullptr || queue[j]->distance() != *prev_distance) {
//                    current_group_begin = j;
//                }
//                if (!recursive && (j == s - 1 || queue[j]->distance() == queue[j+1]->distance())) {
//                    while (j < s - 1 && queue[j]->distance() == queue[j+1]->distance()) {
//                        worker(data, ans, ans->copy_bundle(bundle), queue, current_group_begin, j, edges, visited, true);
//                        ++j;
//                    }
//                    if (j < s) {
//                        worker(data, ans, ans->copy_bundle(bundle), queue, current_group_begin, j, edges, visited, true);
//                    }
//                    ans->delete_bundle(bundle);
//                    break;
//                } else {
//                    if (try_to_add(data, ans, bundle, queue[j], edges, visited)) {
//                        prev_bond = queue[j];
//                    }
//
//                    if (j == s - 1 || queue[j]->distance() != queue[j+1]->distance()) {
//                        recursive = false;
//                    }
//                }
//                prev_distance = &queue[j]->distance();
//            }

//        }
    }

    template <typename T> inline
    bool nearest_with_ret_alg<T>::try_to_add(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, net_bond_ptr<T> bond, std::vector<int32_t>& edges, std::vector<bool>& visited) {
        bool result = false;
        if (edges[0] == -1 && edges[1] == -1) {
            ans->add_bond(bundle, bond);
            edges[0] = bond->from();
            edges[1] = bond->to();

            result = true;
        }

        else if (bond->from() == edges[0] && !visited[bond->to()]) {
            ans->add_bond_front(bundle, &data->at(bond->to(), bond->from()));
            edges[0] = bond->to();

            result = true;
        }

        else if (bond->from() == edges[1] && !visited[bond->to()]) {
            ans->add_bond(bundle, bond);
            edges[1] = bond->to();

            result = true;
        }

        else if (bond->to() == edges[0] && !visited[bond->from()]) {
            ans->add_bond_front(bundle, bond);
            edges[0] = bond->from();

            result = true;
        }

        else if (bond->to() == edges[1] && !visited[bond->from()]) {
            ans->add_bond(bundle, &data->at(bond->to(), bond->from()));
            edges[1] = bond->from();

            result = true;
        }

        if (result) {
            visited[bond->from()] = true;
            visited[bond->to()] = true;
            return true;
        }
        return false;
    }
}

#endif //_CIRCLE_ROUTES_NEAREST_WITH_RET_ALG_H_
