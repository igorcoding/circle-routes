#ifndef _CIRCLE_ROUTES_LITTLE_ALG_H_
#define _CIRCLE_ROUTES_LITTLE_ALG_H_

#include "algorithm.h"

#include <vector>
#include <queue>
#include <utility>
#include <stack>
#include <unordered_map>
#include <algorithm>

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

        return ans;
    }
}

#endif //_CIRCLE_ROUTES_LITTLE_ALG_H_
