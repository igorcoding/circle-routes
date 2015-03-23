#ifndef _CIRCLE_ROUTES_NDATA_H_
#define _CIRCLE_ROUTES_NDATA_H_

#include "net_bond.h"

#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <vector>
#include <limits>

namespace croutes {

    /*************** exceptions ***************/

    class data_inconsistent : public std::exception {
    public:
        data_inconsistent(int32_t from_node, int32_t to_node) :
                _msg("Data is inconsistent in (" + std::to_string(from_node) + ", " + std::to_string(to_node) + ")")
        { }

        virtual const char* what() const noexcept {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };

    class file_not_found : public std::exception {
    public:
        file_not_found(const std::string& filename) :
                _filename(filename),
                _msg("Filename \'" + _filename + "\' was not found")
        { }

        virtual const char* what() const noexcept {
            return _msg.c_str();
        }

        const char* filename() const noexcept {
            return _filename.c_str();
        }

    private:
        std::string _filename;
        std::string _msg;
    };


    /*************** ndata ***************/

    template <typename T>
    class ndata;

    template <typename T>
    using ndata_ptr = std::shared_ptr<ndata<T>>;

    template <typename T>
    class ndata {
    public:
        ndata(size_t nodes_count);
        ndata(const ndata<T>& rhs);
        ~ndata();

        size_t nodes_count() const { return _nodes_count; }
        T inf() const { return _inf; }

        net_bond<T>& at(int32_t from_node, int32_t to_node);
        const net_bond<T>& at(int32_t from_node, int32_t to_node) const;

        net_bond<T>& operator() (int32_t from_node, int32_t to_node);
        const net_bond<T>& operator() (int32_t from_node, int32_t to_node) const;

        void add_node(int32_t from_node, int32_t to_node, T distance);

        ndata_ptr<T> copy() const;

    private:
        void bounds_check(int32_t from_node, int32_t to_node) const;
        ndata<T>& operator= (const ndata<T>& rhs) = delete;

    private:
        size_t _nodes_count;
        T _inf;
        std::vector<std::vector<net_bond<T>*>*>* _matrix;
    };


    template <typename T>
    ndata<T>::ndata(size_t nodes_count)
        : _nodes_count(nodes_count),
          _matrix(new std::vector<std::vector<net_bond<T>*>*>()),
          _inf(std::numeric_limits<T>::infinity()) {
        static_assert(std::numeric_limits<T>::has_infinity, "No infinity value for this type");

        _matrix->reserve(_nodes_count);
        for (int32_t i = 0; i < _nodes_count; ++i) {
            std::vector<net_bond<T>*>* row = new std::vector<net_bond<T>*>();
            row->reserve(_nodes_count);
            for (int32_t j = 0; j < _nodes_count; ++j) {
                row->push_back(new net_bond<T>(i, j, T()));
            }
            _matrix->push_back(row);
        }
    }

    template <typename T>
    ndata<T>::ndata(const ndata<T>& rhs)
            : _nodes_count(rhs._nodes_count),
              _matrix(new std::vector<std::vector<net_bond<T>*>*>()),
              _inf(std::numeric_limits<T>::infinity()) {
        static_assert(std::numeric_limits<T>::has_infinity, "No infinity value for this type");

        _matrix->reserve(_nodes_count);
        for (int32_t i = 0; i < _nodes_count; ++i) {
            std::vector<net_bond<T>*>* row = new std::vector<net_bond<T>*>();
            row->reserve(_nodes_count);
            for (int32_t j = 0; j < _nodes_count; ++j) {
                row->push_back(new net_bond<T>(i, j, rhs.at(i, j).distance()));
            }
            _matrix->push_back(row);
        }
    }

    template <typename T>
    ndata<T>::~ndata() {
        for (size_t i = 0; i < _matrix->size(); ++i) {
            auto s = (*_matrix)[i]->size();
            for (size_t j = 0; j < s; ++j) {
                delete (*(*_matrix)[i])[j];
            }
            delete (*_matrix)[i];
        }
        delete _matrix;
    }

    template <typename T> inline
    net_bond<T>& ndata<T>::at(int32_t from_node, int32_t to_node) {
        return *((*((*_matrix)[from_node]))[to_node]);
    }

    template <typename T> inline
    const net_bond<T>& ndata<T>::at(int32_t from_node, int32_t to_node) const {
        return *((*((*_matrix)[from_node]))[to_node]);
    }

    template <typename T> inline
    net_bond<T>& ndata<T>::operator() (int32_t from_node, int32_t to_node) {
        bounds_check(from_node, to_node);
        return at(from_node, to_node);
    }

    template <typename T> inline
    const net_bond<T>& ndata<T>::operator() (int32_t from_node, int32_t to_node) const {
        bounds_check(from_node, to_node);
        return at(from_node, to_node);
    }

    template <typename T> inline
    void ndata<T>::add_node(int32_t from_node, int32_t to_node, T distance) {
        bounds_check(from_node, to_node);

        if (from_node > to_node) {
            auto rev_dist = at(to_node, from_node).distance();
            if (rev_dist != distance) {
                throw data_inconsistent(from_node, to_node);
            }
        }
        at(from_node, to_node).set_distance(distance);
    }

    template <typename T> inline
    void ndata<T>::bounds_check(int32_t from_node, int32_t to_node) const {
        if (from_node < 0 || from_node >= _nodes_count ||
                to_node < 0 || to_node >= _nodes_count) {
            throw std::out_of_range("Args are out of range");
        }
    }

    template <typename T> inline
    ndata_ptr<T> ndata<T>::copy() const {
        return std::make_shared<ndata<T>>(*this);
    }





    /*************** reading functions ***************/

    template <typename T, typename IS>
    ndata_ptr<T> read_data(IS& is) {
        size_t nodes_count = 0;
        if (is.good()) {
            is >> nodes_count;

            auto d = std::make_shared<ndata<T>>(nodes_count);
            for (size_t i = 0; i < nodes_count; ++i) {
                for (size_t j = 0; j < nodes_count; ++j) {
                    T dist;
                    is >> dist;
                    d->add_node((int32_t) i, (int32_t) j, dist);
                }
            }

            return d;
        }
        throw std::runtime_error("Stream is bad");
    }

    template <typename T>
    ndata_ptr<T> read_console() {
        return read_data<T>(std::cin);
    }

    template <typename T>
    ndata_ptr<T> read_file(const std::string& filename) {
        std::fstream fs;
        fs.open(filename, std::fstream::in);
        if (fs.is_open()) {
            try {
                auto d = read_data<T>(fs);
                fs.close();
                return d;
            } catch (std::exception& e) {
                fs.close();
                throw e;
            }
        } else {
            throw file_not_found(filename);
        }
    }
}

#endif //_CIRCLE_ROUTES_NDATA_H_
