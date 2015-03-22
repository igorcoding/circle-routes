#ifndef _CIRCLE_ROUTES_ALGORITHM_H_
#define _CIRCLE_ROUTES_ALGORITHM_H_

#include "ndata.h"
#include "answer.h"

#include <string>
#include <istream>
#include <memory>

namespace croutes {
    template <typename T>
    class algorithm {
    public:
        algorithm();
        virtual ~algorithm();

        virtual answer_ptr<T> compute(ndata_ptr<T> data);

    protected:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data) = 0;
    };

    template <typename T>
    algorithm<T>::algorithm() {

    }

    template <typename T>
    algorithm<T>::~algorithm() {

    }

    template <typename T>
    answer_ptr<T> algorithm<T>::compute(ndata_ptr<T> data) {
        if (data == nullptr) {
            return nullptr;
        }
        return _compute(data);
    }
}

#endif //_CIRCLE_ROUTES_ALGORITHM_H_
