#pragma once
#include <iostream>
#include <vector>
#include "../../utils/type_descriptor.hpp"
#include "../../ecs/traits.hpp"
#include <memory>

namespace snek::storage
{

    // allows for multiple sparse sets of different component types to be pooled together
    struct polymorphic_sparse_set
    {
        virtual size_t size() const noexcept = 0;
        virtual bool contains(size_t id) const noexcept = 0;
        virtual size_t index(size_t id) const noexcept = 0;
        virtual std::vector<size_t> get_dense() const noexcept = 0;
        virtual void clear() = 0;
        virtual void remove(size_t id) = 0;
        virtual ~polymorphic_sparse_set() {};
    };

#include "../../ecs/traits.hpp"

    template <typename T>
    class sparse_set : public polymorphic_sparse_set
    {
        // where T is a single component type
        std::vector<T> _packed;
        std::vector<size_t> _dense;
        std::vector<size_t> _sparse; // will map the entity id to id in dense domain which is a 1:1 mapping to _packed_elements.
        constexpr static auto tombstone_v = snek::traits::tombstone_t<size_t>::null_v;

    public:
        using value_type = T;

        sparse_set()
        {
            _sparse.resize(1000, tombstone_v);
        };
        sparse_set(size_t initial)
        {
            _sparse.resize(initial, tombstone_v);
        };

        sparse_set(const sparse_set<T> &o) : _dense(o._dense), _sparse(o._sparse) {};

        std::vector<T>::const_iterator begin()
        {
            return _packed.begin();
        };
        std::vector<T>::const_iterator end()
        {
            return _packed.end();
        };

        std::vector<size_t> get_dense() const noexcept override
        {
            return this->_dense;
        };

        void insert(size_t id, T elem)
        {
            if (id > _sparse.size())
            {
                _sparse.resize(_sparse.size() * 2, tombstone_v);
            }
            _sparse[id] = _dense.size();
            _dense.push_back(id);
            _packed.push_back(elem);
        }

        [[nodiscard]] size_t index(size_t id) const noexcept override
        {
            return _dense[_sparse[id]];
        };

        [[nodiscard]] size_t size() const noexcept override
        {

            return _dense.size();
        }
        // check if _dense has elem (id)
        [[nodiscard]] bool contains(size_t elem) const noexcept override
        {
            return (elem < _sparse.size() && _sparse[elem] < _dense.size() && _dense[_sparse[elem]] == elem);
        };

        [[nodiscard]] std::vector<size_t> &get_dense()
        {
            return this->_dense;
        }

        [[nodiscard]] std::vector<T> &get_packed()
        {
            return this->_packed;
        }

        void remove(size_t elem) override
        {
            if (!contains(elem))
                return;
            const auto last = _dense.back();
            std::swap(_dense.back(), _dense[_sparse[elem]]);
            std::swap(_sparse[last], _sparse[elem]);
            _dense.pop_back();
        };
        void clear() override
        {
            this->_dense.clear();
            this->_sparse.clear();
        };

        T *get(size_t id)
        {
            if (!this->contains(id))
                return nullptr;
            return &_packed[_sparse[id]];
        }

        T &get_ref(size_t id)
        {
            if (!this->contains(id))
            {
                throw std::runtime_error("the set does not contain id : " + id);
            }
            return _packed[_sparse[id]];
        }

        ~sparse_set()
        {
            clear();
        };
    };
};
