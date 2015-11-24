#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>

template <typename T>
class Container {
    public:
        ~Container() {
            for (typename std::vector<T*>::iterator i = data_.begin(); i != data_.end(); ++i) {
                delete(*i);
            }
        }

        T* add(T* t) {
            data_.push_back(t);
            return t;
        }

    private:
        std::vector<T*> data_;

};

#endif // CONTAINER_H
