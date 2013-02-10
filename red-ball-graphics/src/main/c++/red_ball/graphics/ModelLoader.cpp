#include "ModelLoader.hpp"

#include <sstream>

#include "red_ball/utils/pointee.hpp"

using namespace red_ball::graphics;

ModelLoaderError::ModelLoaderError(
        const std::string& message,
        const boost::filesystem::path& path,
        std::istream* isPtr
        ) :
    core::RuntimeError(constructMessage_(message, path, isPtr)),
    path_(path),
    offset_(utils::pointee(isPtr).tellg())
{
}

ModelLoaderError::ModelLoaderError(
            const std::string& message,
            const boost::filesystem::path& path,
            std::istream* isPtr,
            const std::exception& cause
            ) :
    core::RuntimeError(constructMessage_(message, path, isPtr), cause),
    path_(path),
    offset_(utils::pointee(isPtr).tellg())
{
}

std::string ModelLoaderError::constructMessage_(
            const std::string& message,
            const boost::filesystem::path& path,
            std::istream* isPtr
            ) {
    std::istream& is = utils::pointee(isPtr);

    size_t offset = is.tellg();

    std::ostringstream oss;
    oss << "Failed to load model file \"" << path.string() << "\". Error at offset " << offset << ": " << message;

    if (is.good()) {
        std::string nextLine;
        std::getline(is, nextLine);
        oss << ". Next line in stream: \"" << nextLine << "\"";
    }

    return oss.str();
}
