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

    private:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node);

    private:

    };

    template <typename T>
    little_alg<T>::little_alg() {

    }

    template <typename T> inline
    answer_ptr<T> little_alg<T>::_compute(ndata_ptr<T> data, int32_t first_node) {
        auto ans = answer<T>::init();
        auto bundle = ans->create_bundle();

        typedef net_bond<T>* bond_t;
        typedef const net_bond<T>* const_bond_t;

        auto comp = [](const bond_t& lhs, const bond_t& rhs) {
            return lhs->distance() < rhs->distance();
        };

        typedef std::priority_queue<bond_t, std::vector<bond_t>, decltype(comp)> pq_t;


        auto size = (int32_t) data->nodes_count();
        auto cdata = data->copy();

        for (int32_t row = 0; row < size; ++row) {
            cdata->at(row, row).distance() = cdata->inf();
        }

        std::vector<bool> skip_rows(data->nodes_count(), false);
        std::vector<bool> skip_cols(data->nodes_count(), false);


        bool continuation = true;
        while (continuation) {

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
//                    std::cout << row << ", " << col << std::endl;
                    return ans;
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
//                    std::cout << row << ", " << col << std::endl;
                    return ans;
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
                    break;
                }
            }

            // TODO: for now getting only first
            bond_t b = max_items[0];
            ans->add_bond(bundle, &data->at(b->from(), b->to()));

            skip_rows[b->from()] = true;
            skip_cols[b->to()] = true;

            auto skipped_rows = std::count(skip_rows.begin(), skip_rows.end(), true);
            auto skipped_cols = std::count(skip_cols.begin(), skip_cols.end(), true);

            if (max_items.size() > 1 && (skipped_rows == size - 1 || skipped_cols == size - 1)) {
                for (size_t i = 1; i < max_items.size(); ++i) {
                    ans->add_bond(bundle, &data->at(max_items[i]->from(), max_items[i]->to()));
                }
                continuation = false;
            } else {
                continuation = std::find(skip_rows.begin(), skip_rows.end(), false) != skip_rows.end() &&
                                std::find(skip_cols.begin(), skip_cols.end(), false) != skip_cols.end();
            }


            if (continuation) {
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

                cdata->at(row, col).distance() = cdata->inf();
            }

            for (auto& b1 : max_items) {
                delete b1;
            }

            while (!pq.empty()) {
                bond_t b1 = pq.top();
                pq.pop();
                delete b1;
            }
        }




        return ans;
    }
}

#endif //_CIRCLE_ROUTES_LITTLE_ALG_H_
