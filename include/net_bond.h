#ifndef _CIRCLE_ROUTES_NET_BOND_H_
#define _CIRCLE_ROUTES_NET_BOND_H_

#include <stdint.h>
#include <ostream>

namespace croutes {
    template <typename T>
    class net_bond {
    public:
        net_bond(int32_t from, int32_t to, T distance);


        int32_t from() const { return _from; }
        int32_t to() const { return _to; }
        const T& distance() const { return _distance;  }


        void set_from(int32_t from) { _from = from; }
        void set_to(int32_t to) { _to = to; }
        void set_distance(T distance) { _distance = distance; }

    private:
        int32_t _from;
        int32_t _to;
        T _distance;
    };

    template <typename T>
    net_bond<T>::net_bond(int32_t from, int32_t to, T distance)
        : _from(from),
          _to(to),
          _distance(distance) {

    }

    template <typename T>
    std::ostream& operator <<(std::ostream& os, const net_bond<T>& b) {
        os << b.from() << " --" << b.distance() << "--> " << b.to();
        return os;
    }
 }

#endif //_CIRCLE_ROUTES_NET_BOND_H_
