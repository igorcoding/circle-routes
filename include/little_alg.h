#ifndef _CIRCLE_ROUTES_LITTLE_ALG_H_
#define _CIRCLE_ROUTES_LITTLE_ALG_H_

#include "algorithm.h"

#include <vector>
#include <queue>
#include <utility>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <assert.h>

namespace croutes {

    template <typename T>
    class little_alg : public algorithm<T> {
    public:
        little_alg();

        virtual const std::string& name() override;

    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node) override;
        void worker(ndata_ptr<T> data, ndata_ptr<T> cdata1, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, std::vector<bool> skip_rows, std::vector<bool> skip_cols);

    private:
        std::string _name;
    };


    template <typename T>
    little_alg<T>::little_alg()
            : _name("Little\'s algorithm") {

    }

    template <typename T> inline
    const std::string& little_alg<T>::name() {
        return _name;
    }

    template <typename T>
    void little_alg<T>::worker(ndata_ptr<T> data, ndata_ptr<T> cdata1, answer_ptr<T> ans, typename answer<T>::bundle_t* bundle, std::vector<bool> skip_rows, std::vector<bool> skip_cols) {
        auto size = (int32_t) data->nodes_count();
        auto cdata = cdata1->copy();

        typedef net_bond<T>* bond_t;
        auto comp = [](const bond_t& lhs, const bond_t& rhs) {
            return lhs->distance() < rhs->distance();
        };

        typedef std::priority_queue<bond_t, std::vector<bond_t>, decltype(comp)> pq_t;


        while (true) {

            // subtracting minimum in rows
            for (int32_t row = 0; row < size; ++row) {
                T min_dist = cdata->inf();
                for (int32_t col = 0; col < size; ++col) {
                    if (!skip_cols[col]) {
                        auto n = &cdata->at(row, col);
                        if (n->distance() < min_dist) {
                            min_dist = n->distance();
                        }
                    }
                }

                if (min_dist == cdata->inf()) {
                    return;
                }

                if (min_dist != 0) {
                    for (int32_t col = 0; col < size; ++col) {
                        if (!skip_cols[col]) {
                            cdata->at(row, col).distance() -= min_dist;
                        }
                    }
                }
            }

            // subtracting minimum in columns
            for (int32_t col = 0; col < size; ++col) {
                T min_dist = cdata->inf();
                for (int32_t row = 0; row < size; ++row) {
                    if (!skip_rows[row]) {
                        auto n = &cdata->at(row, col);
                        if (n->distance() < min_dist) {
                            min_dist = n->distance();
                        }
                    }
                }

                if (min_dist == cdata->inf()) {
                    return;
                }

                if (min_dist != 0) {
                    for (int32_t row = 0; row < size; ++row) {
                        if (!skip_rows[row]) {
                            cdata->at(row, col).distance() -= min_dist;
                        }
                    }
                }
            }

            pq_t pq(comp);

            // counting sum of minimums at zero elements
            for (int32_t row = 0; row < size; ++row) {
                if (skip_rows[row])
                    continue;

                for (int32_t col = 0; col < size; ++col) {
                    if (skip_cols[col])
                        continue;

                    auto n = &cdata->at(row, col);
                    if (n->distance() == 0) {
                        T min_in_row = cdata->inf();
                        T min_in_col = cdata->inf();

                        // finding minimum in row
                        for (int32_t col1 = 0; col1 < size; ++col1) {
                            auto n1 = &cdata->at(row, col1);
                            if (col1 != col && !skip_cols[col1] && n1->distance() < min_in_row) {
                                min_in_row = n1->distance();
                            }
                        }

                        // finding minimum in col
                        for (int32_t row1 = 0; row1 < size; ++row1) {
                            auto n1 = &cdata->at(row1, col);
                            if (row1 != row && !skip_rows[row1] && n1->distance() < min_in_col) {
                                min_in_col = n1->distance();
                            }
                        }

                        pq.push(new net_bond<T>(row, col, min_in_row + min_in_col));
                    }
                }
            }

            std::vector<bond_t> max_items;

            while (!pq.empty()) {
                if (max_items.empty() || max_items.back()->distance() == pq.top()->distance()) {
                    max_items.push_back(pq.top());
                    pq.pop();
                } else {
                    bond_t b1 = pq.top();
                    pq.pop();
                    delete b1;
                }
            }


            if (max_items[0]->distance() == cdata->inf()) {
                for (auto& b : max_items) {
                    ans->add_bond(bundle, &data->at(b->from(), b->to()));
                    delete b;
                }
                break;
            }
            bool recursive = max_items.size() > 1;
            auto old_bundle = bundle;
            for (size_t k = 0; k < max_items.size(); ++k) {
                auto& b = max_items[k];
                if (max_items.size() > 1) {
                    bundle = ans->copy_bundle(old_bundle);
                }
                ans->add_bond(bundle, &data->at(b->from(), b->to()));


                skip_rows[b->from()] = true;
                skip_cols[b->to()] = true;

                int32_t row = 0;
                for (row = 0; row < size; ++row) {
                    if (skip_rows[row])
                        continue;
                    int32_t col = 0;
                    for (col = 0; col < size; ++col) {
                        if (skip_cols[col])
                            continue;

                        if (cdata->at(row, col).distance() == cdata->inf()) {
                            break;
                        }
                    }

                    if (col >= size) {
                        break;
                    }
                }

                int32_t col = 0;
                for (col = 0; col < size; ++col) {
                    if (skip_cols[col])
                        continue;
                    int32_t row1 = 0;
                    for (row1 = 0; row1 < size; ++row1) {
                        if (skip_rows[row1])
                            continue;

                        if (cdata->at(row1, col).distance() == cdata->inf()) {
                            break;
                        }
                    }

                    if (row1 >= size) {
                        break;
                    }
                }
                auto prev_dist = cdata->at(row, col).distance();
                if (row < size && col < size) {
                    cdata->at(row, col).distance() = cdata->inf();
                }

                if (recursive) {
                    worker(data, cdata, ans, bundle, skip_rows, skip_cols);

                    skip_rows[b->from()] = false;
                    skip_cols[b->to()] = false;
                    if (row < size && col < size) {
                        cdata->at(row, col).distance() = prev_dist;
                    }
                }
            }
            for (auto& b1 : max_items) {
                delete b1;
            }
            if (recursive) {
                ans->delete_bundle(old_bundle);
                break;
            }
        }
    }

    template <typename T> inline
    answer_ptr<T> little_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node) {
        auto ans = answer<T>::init();
        auto bundle = ans->create_bundle();

        auto size = (int32_t) data->nodes_count();

        auto cdata = data->copy();

        for (int32_t row = 0; row < size; ++row) {
            cdata->at(row, row).distance() = cdata->inf();
        }

        std::vector<bool> skip_rows(cdata->nodes_count(), false);
        std::vector<bool> skip_cols(cdata->nodes_count(), false);
        worker(data, cdata, ans, bundle, skip_rows, skip_cols);
        return ans;
    }
}

#endif //_CIRCLE_ROUTES_LITTLE_ALG_H_
