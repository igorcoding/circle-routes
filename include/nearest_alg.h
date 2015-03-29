#ifndef _CIRCLE_ROUTES_NEAREST_ALG_H_
#define _CIRCLE_ROUTES_NEAREST_ALG_H_


#include "algorithm.h"

#include <queue>
#include <algorithm>
#include <assert.h>

namespace croutes {
    template <typename T>
    class nearest_alg : public algorithm<T> {
    public:
        nearest_alg();

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
    nearest_alg<T>::nearest_alg()
            : _short_name("nearest_alg"),
              _name("To the nearset algorithm"),
              _russian_name("Алгоритм \"Иди в ближний\"") {

    }

    template <typename T> inline
    answer_ptr<T> nearest_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node_) {
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

        return ans;
    }

    template <typename T> inline
    void nearest_alg<T>::worker(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, const std::vector<net_bond_ptr<T>>& queue, int32_t group_begin, int32_t current, std::vector<int32_t> edges, std::vector<bool> visited, bool recursive) {
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
                }

            }
        }
    }

    template <typename T> inline
    bool nearest_alg<T>::try_to_add(ndata_ptr<T> data, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, net_bond_ptr<T> bond, std::vector<int32_t>& edges, std::vector<bool>& visited) {
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

#endif //_CIRCLE_ROUTES_NEAREST_ALG_H_
