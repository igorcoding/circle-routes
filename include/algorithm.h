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

        virtual answer_ptr<T> compute(ndata_ptr<T> data, int32_t first_node = 0);
        virtual const std::string& short_name() = 0;
        virtual const std::string& name() = 0;
        virtual const std::string& russian_name() = 0;


    protected:
        virtual answer_ptr<T> _compute(ndata_ptr<T> data, int32_t first_node) = 0;
    };

    template <typename T>
    algorithm<T>::algorithm() {
        static_assert(!std::is_integral<T>::value, "Integral types are not allowed");
    }

    template <typename T>
    algorithm<T>::~algorithm() {

    }

    template <typename T>
    answer_ptr<T> algorithm<T>::compute(ndata_ptr<T> data, int32_t first_node) {
        if (data == nullptr) {
            return nullptr;
        }
        return _compute(data, first_node);
    }
}

#endif //_CIRCLE_ROUTES_ALGORITHM_H_
