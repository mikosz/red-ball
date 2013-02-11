#include "ObjFileLoader.hpp"

#include <fstream>
#include <sstream>
#include <iterator>

#include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "red_ball/utils/pointee.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;
namespace phoenix = boost::phoenix;

struct ObjectVertex {

    size_t positionIndex;

    size_t textureCoordinateIndex;

    size_t normalIndex;

};

struct ObjectFace {

    ObjectVertex vertices[3];

};

struct Object {

    std::string name;

    std::vector<ObjectFace> faces;

};

class ParserError : public core::RuntimeError {
public:

    ParserError(const std::string& message) :
        core::RuntimeError(message)
    {
    }

};

template <class Iterator, class SkipperType>
class ObjFileParser : public qi::grammar<Iterator, void(), SkipperType> {
public:

    typedef std::vector<D3DXVECTOR3> Positions;

    typedef std::vector<D3DXVECTOR2> TextureCoordinates;

    typedef std::vector<D3DXVECTOR3> Normals;

    typedef std::vector<Object> Objects;

    ObjFileParser() :
        ObjFileParser::base_type(startRule_)
    {
        blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
        ruleEndRule_ = (qi::eol | qi::eoi) >> *blankRule_;
        smoothingGroupRule_ = 's' >> (qi::lit("off") | qi::int_) >> ruleEndRule_;
        materialRule_ = qi::lit("usemtl") >> (*(qi::char_ - ascii::space)) >> ruleEndRule_;
        objectVertexRule_ = (qi::uint_ % '/')[qi::_val = phoenix::bind(&ObjFileParser::makeObjectVertex, this, qi::_1)];
        faceRule_ = 'f' >> qi::repeat(3)[objectVertexRule_][boost::bind(&ObjFileParser::addFace, this, _1)]
                >> ruleEndRule_;
        positionRule_ = 'v' >> qi::repeat(3)[qi::double_][boost::bind(&ObjFileParser::addPosition, this, _1)]
                >> ruleEndRule_;
        textureCoordinateRule_ = qi::lit("vt")
                >> qi::repeat(2)[qi::double_][boost::bind(&ObjFileParser::addTextureCoordinate, this, _1)]
                >> ruleEndRule_;
        normalRule_ = qi::lit("vn")
                >> qi::repeat(3)[qi::double_][boost::bind(&ObjFileParser::addNormal, this, _1)]
                >> ruleEndRule_;
        objectNameRule_ = (qi::char_('o') | 'g')
                >> (*(qi::char_ - ascii::space))[boost::bind(&ObjFileParser::newObject, this, _1)]
                >> ruleEndRule_;
        materialLibRule_ = qi::lit("mtllib") >> (*(qi::char_ - ascii::space)) >> ruleEndRule_;

        startRule_ = *blankRule_
                >> *(
                        smoothingGroupRule_ |
                        materialRule_ |
                        materialLibRule_ |
                        faceRule_ |
                        positionRule_ |
                        textureCoordinateRule_ |
                        normalRule_ |
                        objectNameRule_
                        );
    }

    const Positions& positions() const {
        return positions_;
    }

    const TextureCoordinates& textureCoordinates() const {
        return textureCoordinates_;
    }

    const Normals& normals() const {
        return normals_;
    }

    const Objects& objects() const {
        return objects_;
    }

private:

    typedef qi::rule<Iterator, void (), SkipperType> Rule;

    Rule blankRule_;

    Rule ruleEndRule_;

    Rule smoothingGroupRule_;

    Rule materialRule_;

    qi::rule<Iterator, ObjectVertex(), SkipperType> objectVertexRule_;

    Rule faceRule_;

    Rule positionRule_;

    Rule textureCoordinateRule_ ;

    Rule normalRule_;

    Rule objectNameRule_;

    Rule materialLibRule_;

    Rule startRule_;

    Positions positions_;

    TextureCoordinates textureCoordinates_;

    Normals normals_;

    Objects objects_;

    static void vectorToString(std::string* stringParam, const std::vector<char>& vector) {
        std::string& string = utils::pointee(stringParam);
        std::copy(vector.begin(), vector.end(), std::back_inserter(string));
    }

    void addPosition(const std::vector<double>& vector) {
        assert(vector.size() == 3);
        positions_.push_back(D3DXVECTOR3(vector[0], vector[1], vector[2]));
    }

    void addTextureCoordinate(const std::vector<double>& vector) {
        assert(vector.size() == 2);
        textureCoordinates_.push_back(D3DXVECTOR2(vector[0], vector[1]));
    }

    void addNormal(const std::vector<double>& vector) {
        assert(vector.size() == 3);
        normals_.push_back(D3DXVECTOR3(vector[0], vector[1], vector[2]));
    }

    void newObject(const std::vector<char>& chars) {
        objects_.push_back(Object());
        vectorToString(&objects_.back().name, chars);
    }

    ObjectVertex makeObjectVertex(const std::vector<size_t>& objectVertexData) {
        if (objectVertexData.size() != 3) {
            throw ParserError("Expected object vertex data to contain 3 elements");
        }

        ObjectVertex result;
        result.positionIndex = objectVertexData[0] - 1;
        result.textureCoordinateIndex = objectVertexData[1] - 1;
        result.normalIndex = objectVertexData[2] - 1;

        return result;
    }

    void addFace(const std::vector<ObjectVertex>& face) {
        if (face.size() != 3) {
            throw ParserError("Currently supporting only faces with 3 vertices");
        }

        if (objects_.empty()) {
            throw ParserError("Attempted to add a face with no object specified");
        }

        ObjectFace storedFace;
        storedFace.vertices[0] = face[0];
        storedFace.vertices[1] = face[1];
        storedFace.vertices[2] = face[2];

        objects_.back().faces.push_back(storedFace);
    }

};

void toLeftHandSystem(Vertex* vertexPtr) {
    Vertex& vertex = utils::pointee(vertexPtr);

    vertex.position.z *= -1.0f;
    vertex.textureCoordinates.y = 1.0f - vertex.textureCoordinates.y;
    vertex.normal.z *= -1.0f;
}

void toClockwise(unsigned long*, unsigned long* yPtr, unsigned long* zPtr) {
    unsigned long& y = utils::pointee(yPtr);
    unsigned long& z = utils::pointee(zPtr);

    std::swap(y, z);
}

void load(
        std::istream* isPtr,
        std::vector<Vertex>* verticesPtr,
        std::vector<unsigned long>* indicesPtr,
        const boost::filesystem::path& path
        ) {
    typedef ObjFileParser<spirit::istream_iterator, ascii::blank_type> Parser;

    std::istream& is = utils::pointee(isPtr);
    std::vector<Vertex>& vertices = utils::pointee(verticesPtr);
    std::vector<unsigned long>& indices = utils::pointee(indicesPtr);

    is.unsetf(std::istream::skipws);
    Parser parser;
    spirit::istream_iterator it(is), end;

    try {
        bool result = qi::phrase_parse(
                    it,
                    end,
                    parser,
                    ascii::blank
                    );
        if (!result || it != end) {
            throw ModelLoaderError("Failed to parse input", path, &is);
        }
    } catch (const ParserError& e) {
        throw ModelLoaderError("Caught a parser error", path, &is, e);
    }

    // TODO: this code is silly - it does not actually use the index buffer, because all the vertices
    // are copied into the vector
    try {
        bool hasFaces = false;
        const Parser::Objects& objects = parser.objects();
        for (size_t i = 0; i < objects.size(); ++i) {
            hasFaces = hasFaces || !objects[i].faces.empty();
            for (size_t j = 0; j < objects[i].faces.size(); ++j) {
                const ObjectFace& face = objects[i].faces[j];
                for (size_t k = 0; k < 3; ++k) {
                    vertices.push_back(Vertex());
                    Vertex& vertex = vertices.back();
                    vertex.position = parser.positions().at(face.vertices[k].positionIndex);
                    vertex.normal = parser.normals().at(face.vertices[k].normalIndex);
                    vertex.textureCoordinates = parser.textureCoordinates().at(
                            face.vertices[k].textureCoordinateIndex);
                    // TODO: assuming right hand system in file, should determine somehow
                    toLeftHandSystem(&vertex);

                    indices.push_back(vertices.size() - 1);
                }

                // TODO: assuming counter clockwise in file, should determine somehow
                toClockwise(&indices[vertices.size() - 3], &indices[vertices.size() - 2], &indices[vertices.size() - 1]);
            }
        }

        if (!hasFaces) {
            throw ModelLoaderError("No faces found for model", path, &is);
        }
    } catch (const std::out_of_range& e) {
        throw ModelLoaderError("Found invalid index in face", path, &is, e);
    }
}

} // anonymous namespace

ObjFileLoader::ObjFileLoader(const boost::filesystem::path& path) :
        vertices_(new std::vector<Vertex>),
        indices_(new std::vector<unsigned long>)
{
    std::ifstream ifs;
    ifs.exceptions(std::ios::badbit);
    try {
        ifs.open(path.string().c_str());
        load(&ifs, vertices_.get(), indices_.get(), path);
    } catch (const std::istream::failure& e) {
        std::ostringstream error;
        error << "Failure while reading stream - " << e.what();
        throw ModelLoaderError(error.str(), path, &ifs, e);
    }
}

ObjFileLoader::ObjFileLoader(std::istream* isPtr) :
        vertices_(new std::vector<Vertex>),
        indices_(new std::vector<unsigned long>)
{
    std::istream& is = utils::pointee(isPtr);
    is.exceptions(std::ios::badbit);
    load(&is, vertices_.get(), indices_.get(), boost::filesystem::path());
}

boost::shared_ptr<const std::vector<Vertex> > ObjFileLoader::vertices() const {
    return vertices_;
}

boost::shared_ptr<const std::vector<unsigned long> > ObjFileLoader::indices() const {
    return indices_;
}
