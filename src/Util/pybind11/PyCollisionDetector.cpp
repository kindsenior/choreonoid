/*!
  @author Kunio Kojima
*/

#include "../CollisionDetector.h"
#include <cnoid/PyEigenTypes>
#include <pybind11/pybind11.h>

using namespace cnoid;
namespace py = pybind11;

namespace cnoid {

void exportPyCollisionDetector(py::module& m)
{
    py::class_<Collision>(m, "Collision")
        .def_readwrite("point", &Collision::point)
        .def_readwrite("normal", &Collision::normal)
        .def_readwrite("depth", &Collision::depth)
        ;
}

}
