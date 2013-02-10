#ifndef RED_BALL_GRAPHICS_OBJFILELOADER_HPP_
#define RED_BALL_GRAPHICS_OBJFILELOADER_HPP_

#include <iosfwd>

#include <boost/filesystem/path.hpp>

#include "ModelLoader.hpp"

namespace red_ball {
namespace graphics {

class ObjFileLoader : public ModelLoader {
public:

    ObjFileLoader(const boost::filesystem::path& path);

    ObjFileLoader(std::istream* is);

    boost::shared_ptr<const std::vector<Vertex> > vertices() const;

    boost::shared_ptr<const std::vector<unsigned long> > indices() const;

private:

    boost::shared_ptr<std::vector<Vertex> > vertices_;

    boost::shared_ptr<std::vector<unsigned long> > indices_;

};

} // namespace graphics
} // namespace red_ball

#endif /* RED_BALL_GRAPHICS_OBJFILELOADER_HPP_ */
