#include "primitives/mesh.h"
#include "primitives/triangle-primitive.h"
#include "primitives/union-primitive.h"
#include "primitives/bounding-box-accelerator-primitive.h"
#include <list>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include "easylogging++.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{

	class MeshImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
    private:
        Primitive m_mesh;

        Primitive parse_triangle(std::istream& mesh_data_stream)
        {
            long double x, y, z;
            mesh_data_stream >> x >> y >> z;
            LOG(INFO) << std::setprecision(18);
            LOG(INFO) << x << " -- " << y << " -- " << z;
            auto p1 = Point3D(x, y, z);
            mesh_data_stream >> x >> y >> z;
            LOG(INFO) << x << " -- " << y << " -- " << z;
            auto p2 = Point3D(x, y, z);
            mesh_data_stream >> x >> y >> z;
            LOG(INFO) << x << " -- " << y << " -- " << z;
            auto p3 = Point3D(x, y, z);

            return triangle(p1, p2, p3);
        }

        Primitive parse(std::istream& mesh_data_stream)
        {
            std::string command;
            std::vector<Primitive> triangle_stack;
            std::vector<Primitive> box_stack;
            while(mesh_data_stream >> command)
            {
                LOG(INFO) << command;
                if (command == "triangle")
                {
                    triangle_stack.push_back(parse_triangle(mesh_data_stream));
                }
                else if(triangle_stack.size() > 0)
                {
                    assert(command == "box");
                    auto tmp = make_union(triangle_stack);
                    tmp = bounding_box_accelerator(tmp);
                    triangle_stack.clear();
                    box_stack.push_back(tmp);
                }
                else if(command == "emptybox")
                {
                    box_stack.push_back(empty_box());
                }
                else
                {
                    assert(command == "box");
                    assert(box_stack.size() >= 2);

                    std::vector<Primitive> toptwo;
                    toptwo.push_back(box_stack.back());
                    box_stack.pop_back();
                    toptwo.push_back(box_stack.back());
                    box_stack.pop_back();

                    auto tmp = make_union(toptwo);
                    tmp = bounding_box_accelerator(tmp);
                    box_stack.push_back(tmp);
                }
            }

            assert(triangle_stack.empty());
            assert(box_stack.size() == 1);
            return box_stack[0];
        }

    public:

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="primitive">
		/// primitive
		/// </param>
 
        MeshImplementation(std::istream& mesh_data_stream)
        {
            m_mesh = parse(mesh_data_stream);
        }

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			return m_mesh->find_all_hits(ray); // TODO
		}

		Box bounding_box() const override
		{
			return m_mesh->bounding_box(); // TODO
		}

	};

}

// Primitive raytracer::primitives::Primitive mesh(istream& mesh_data_stream)
Primitive raytracer::primitives::mesh(const std::string& filename)
{
    std::ifstream mesh_data_stream(filename);
    LOG(INFO) << filename;
	return Primitive(std::make_shared<MeshImplementation>(mesh_data_stream));
}
