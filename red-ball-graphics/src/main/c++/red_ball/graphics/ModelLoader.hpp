#ifndef RED_BALL_GRAPHICS_MODELLOADER_HPP_
#define RED_BALL_GRAPHICS_MODELLOADER_HPP_

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include "red_ball/core/exceptions.hpp"
#include "Vertex.hpp"

namespace red_ball {
namespace graphics {

class ModelLoaderError : public core::RuntimeError {
public:

    ModelLoaderError(
            const std::string& message,
            const boost::filesystem::path& path,
            std::istream* is
            );

    ModelLoaderError(
            const std::string& message,
            const boost::filesystem::path& path,
            std::istream* is,
            const std::exception& cause
            );

    const boost::filesystem::path& path() const {
        return path_;
    }

    size_t offset() const {
        return offset_;
    }

private:

    boost::filesystem::path path_;

    size_t offset_;

    static std::string constructMessage_(
            const std::string& message,
            const boost::filesystem::path& path,
            std::istream* is
            );

};

class ModelLoader {
public:

    virtual ~ModelLoader() {
    }

    virtual boost::shared_ptr<const std::vector<Vertex> > vertices() const = 0;

    virtual boost::shared_ptr<const std::vector<unsigned long> > indices() const = 0;

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_MODELLOADER_HPP_ */
