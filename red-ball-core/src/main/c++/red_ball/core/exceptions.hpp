#ifndef RED_BALL_CORE_EXCEPTIONS_HPP_
#define RED_BALL_CORE_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>
#include <sstream>

namespace red_ball {
namespace core {

template <class Super>
class GenericException : public Super {
public:

    GenericException(const std::string& message) :
        Super(message),
        message_(message) {
    }

    GenericException(const std::string& message, const std::exception& cause) :
        Super(messageWithCause(message, cause)) {
    }

    ~GenericException() throw () {
    }

    const std::string& message() const {
        return message_;
    }

private:

    std::string message_;

    static std::string messageWithCause(const std::string& message, const std::exception& cause) {
        std::ostringstream oss;
        oss << message << "\n-- Caused by:\n" << cause.what();
        return oss.str();
    }

};

typedef GenericException<std::runtime_error> RuntimeError;

typedef GenericException<std::logic_error> LogicError;

}  // namespace core
}  // namespace red_ball

#endif /* RED_BALL_CORE_EXCEPTIONS_HPP_ */
