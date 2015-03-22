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
    class answer;

    template <typename T>
    using answer_ptr = std::shared_ptr<answer<T>>;

    template <typename T>
    class answer {
    public:
        typedef std::vector<const net_bond<T>*> bundle_t;

        answer();
        ~answer();

        const std::vector<bundle_t*>& bundles() const { return _bundles; }

        bundle_t* create_bundle();
        bundle_t* copy_bundle(bundle_t* bundle);
        void add_bond(bundle_t* bundle, const net_bond<T>* bond);
        T total_distance(bundle_t* bundle) const;

        static answer_ptr<T> init();

    private:
        std::vector<bundle_t*> _bundles;
    };

    template <typename T>
    answer_ptr<T> answer<T>::init() {
        return std::make_shared<answer<T>>();
    }

    template <typename T>
    answer<T>::answer() {
    }

    template <typename T>
    answer<T>::~answer() {
        for (auto& b : _bundles) {
            delete b;
            b = nullptr;
        }
    }

    template <typename T> inline
    typename answer<T>::bundle_t* answer<T>::create_bundle() {
        _bundles.push_back(new bundle_t());
        return _bundles.back();
    }

    template <typename T> inline
    typename answer<T>::bundle_t* answer<T>::copy_bundle(bundle_t* bundle) {
        _bundles.push_back(new bundle_t(bundle));
        return _bundles.back();
    }

    template <typename T> inline
    void answer<T>::add_bond(bundle_t* bundle, const net_bond<T>* bond) {
        bundle->push_back(bond);
    }

    template <typename T> inline
    T answer<T>::total_distance(bundle_t* bundle) const {
        T dist = T();
        for (auto& b : *bundle) {
            dist += b->distance();
        }
        return dist;
    }
}

#endif //_CIRCLE_ROUTES_ANSWER_H_
