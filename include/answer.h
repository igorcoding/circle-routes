#ifndef _CIRCLE_ROUTES_ANSWER_H_
#define _CIRCLE_ROUTES_ANSWER_H_

#include "net_bond.h"

#include <vector>
#include <stdint.h>
#include <set>
#include <stdexcept>
#include <memory>

namespace croutes {
    template <typename T>
    class answer {
    public:

        answer();

        const std::vector<const net_bond<T>*>& bonds() const { return _bonds; }

        void add_bond(const net_bond<T>* bond);
        T total_distance() const;

    private:
        std::vector<const net_bond<T>*> _bonds;
    };

    template <typename T>
    using answer_ptr = std::shared_ptr<answer<T>>;

    template <typename T>
    answer<T>::answer() {
    }

    template <typename T> inline
    void answer<T>::add_bond(const net_bond<T>* bond) {
        _bonds.push_back(bond);
    }

    template <typename T> inline
    T answer<T>::total_distance() const {
        T dist = T();
        for (auto& b : _bonds) {
            dist += b->distance();
        }
        return dist;
    }
}

#endif //_CIRCLE_ROUTES_ANSWER_H_
