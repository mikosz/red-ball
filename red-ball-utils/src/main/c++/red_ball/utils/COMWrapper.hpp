#ifndef RED_BALL_UTILS_COMWRAPPER_HPP_
#define RED_BALL_UTILS_COMWRAPPER_HPP_

#include <boost/noncopyable.hpp>

namespace red_ball {
namespace utils {

template <class T>
class COMWrapper : boost::noncopyable {
public:

    COMWrapper() :
        comObject_(0) {
    }

    explicit COMWrapper(T* comObject) :
            comObject_(comObject) {
    }

    ~COMWrapper() {
        reset();
    }

    operator T* () const {
        return comObject_;
    }

    T* operator-> () const {
        return comObject_;
    }

    T& operator* () const {
        return *comObject_;
    }

    void reset() {
        if (comObject_) {
            comObject_->Release();
            comObject_ = 0;
        }
    }

    void reset(T* comObject) {
        reset();
        comObject_ = comObject;
    }

    T*& get() {
        return comObject_;
    }

    T* const & get() const {
        return comObject_;
    }

    T* release() {
        T* comObject = comObject_;
        comObject_ = 0;
        return comObject;
    }

private:

    T* comObject_;

};

}  // namespace utils
}  // namespace red_ball

#endif /* RED_BALL_UTILS_COMWRAPPER_HPP_ */
