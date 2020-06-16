/*!
  @author Shin'ichiro Nakaoka
*/

#include "../PoseSeqItem.h"
#include <cnoid/PyBase>
#include <pybind11/stl.h>

using namespace cnoid;
namespace py = pybind11;

PYBIND11_MODULE(PoseSeqPlugin, m)
{
    m.doc() = "Choreonoid PoseSeqPlugin module";

    py::module::import("cnoid.BodyPlugin");

    py::class_<PoseSeqItem, PoseSeqItemPtr, Item>(m, "PoseSeqItem")
        .def("poseSeq", &PoseSeqItem::poseSeq)
        .def("bodyMotionItem", &PoseSeqItem::bodyMotionItem)
        .def("beginEditing", &PoseSeqItem::beginEditing)
        .def("endEditing", &PoseSeqItem::endEditing, py::arg("actuallyModified") = true)
        ;

    py::class_<PoseRef>(m, "PoseRef")
        .def("getPose", [](PoseRef& self){ return self.get<Pose>(); })
        .def("time", &PoseRef::time)
        ;

    py::class_<PoseUnit, PoseUnitPtr, Referenced>(m, "PoseUnit");

    py::class_<PoseSeq, PoseSeqPtr, PoseUnit>(m, "PoseSeq")
        .def("__len__", [](PoseSeq& self) { return self.size(); })
        .def("__iter__", [](PoseSeq& self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>())
        .def("front", &PoseSeq::front)
        .def("back", &PoseSeq::back)
        .def("beginPoseModification", [](PoseSeq& self, PoseRef& poseRef){ self.beginPoseModification(self.seek(self.begin(), poseRef.time())); })
        .def("endPoseModification", [](PoseSeq& self, PoseRef& poseRef){ self.endPoseModification(self.seek(self.begin(), poseRef.time())); })
        ;

    py::class_<Pose, PosePtr, PoseUnit>(m, "Pose")
        .def_property("numJoints", &Pose::numJoints, &Pose::setNumJoints)
        .def("setJointPosition", &Pose::setJointPosition)
        .def("jointPosition", &Pose::jointPosition)
        .def("isJointValid", &Pose::isJointValid)
        .def_property_readonly("numIkLinks", &Pose::numIkLinks)
        .def("ikLinkInfo", (Pose::LinkInfo*(Pose::*)(int))&Pose::ikLinkInfo, py::return_value_policy::reference)
        .def("baseLinkIndex", &Pose::baseLinkIndex)
        .def("baseLinkInfo", &Pose::baseLinkInfo)
        .def("__iter__", [](Pose& self) { return py::make_iterator(self.ikLinkBegin(), self.ikLinkEnd()); }, py::keep_alive<0, 1>())
        ;

    py::class_<Pose::LinkInfo>(m, "LinkInfo")
        .def_readwrite("p", &Pose::LinkInfo::p)
        .def_readwrite("R", &Pose::LinkInfo::R)
        .def("isBaseLink", &Pose::LinkInfo::isBaseLink)
        .def("setStationaryPoint", &Pose::LinkInfo::setStationaryPoint)
        .def("isStationaryPoint", &Pose::LinkInfo::isStationaryPoint)
        .def("isTouching", &Pose::LinkInfo::isTouching)
        .def("partingDirection", &Pose::LinkInfo::partingDirection)
        .def("contactPoints", &Pose::LinkInfo::contactPoints)
        .def("setTouching", &Pose::LinkInfo::setTouching)
        .def("clearTouching", &Pose::LinkInfo::clearTouching)
        .def("isSlave", &Pose::LinkInfo::isSlave)
        .def("setSlave", &Pose::LinkInfo::setSlave)
        ;
}
