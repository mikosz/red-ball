#ifndef RED_BALL_GRAPHICS_DISPLAYINFO_HPP_
#define RED_BALL_GRAPHICS_DISPLAYINFO_HPP_

#include <string>
#include <utility>

#include "exceptions.hpp"

namespace red_ball {
namespace graphics {

class DisplayInfo {
public:

    typedef std::pair<int, int> Resolution;

    typedef std::pair<unsigned int, unsigned int> RefreshRate;

    struct VideoCardInfo {

        size_t memory;

        std::wstring name;

    };

    DisplayInfo();

    const Resolution& resolution() const {
        return resolution_;
    }

    const RefreshRate& refreshRate() const {
        return refreshRate_;
    }

private:

    Resolution resolution_;

    RefreshRate refreshRate_;

    VideoCardInfo videoCard_;

};

class DisplayInfoDiscoveryError : public DirectXException {
public:

    DisplayInfoDiscoveryError(HRESULT result, const std::string& message);

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_DISPLAYINFO_HPP_ */
