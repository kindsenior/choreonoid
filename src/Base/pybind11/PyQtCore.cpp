/*!
  @author Shin'ichiro Nakaoka
*/

#include "PyQString.h"
#include <QEventLoop>
#include <QObject>
#include <QTimer>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(QtCore, m)
{
    m.doc() = "Choreonoid QtCore module";

    py::class_<QEventLoop>qEventLoop(m,"QEventLoop");
    qEventLoop.def(py::init<>())
        // .def(py::init<QObject*>(), py::arg("parent")=(QObject*)nullptr )
        // .def(py::init<QObject*>(), py::arg("parent")=(QObject*)(0) )
        // .def("processEvents", (bool (QEventLoop::*)(QEventLoop::ProcessEventsFlags)) &QEventLoop::processEvents, py::arg("flags")=QEventLoop::AllEvents)
        .def("processEvents", [](QEventLoop& self){ return self.processEvents(); })
        ;

    // py::enum_<QEventLoop::ProcessEventsFlag>(qEventLoop, "ProcessEventsFlag")
    //     .value("AllEvents", QEventLoop::ProcessEventsFlag::AllEvents)
    //     .value("ExcludeUserInputEvents", QEventLoop::ProcessEventsFlag::ExcludeUserInputEvents)
    //     .value("ExcludeSocketNotifiers", QEventLoop::ProcessEventsFlag::ExcludeSocketNotifiers)
    //     .value("WaitForMoreEvents", QEventLoop::ProcessEventsFlag::WaitForMoreEvents)
    //     .export_values();

    // py::class_<QEventLoop::ProcessEventsFlags>(m, "ProcessEventsFlags")
    //     .def(py::init<>());

    py::class_<QObject>(m,"QObject")
        .def("blockSignals", &QObject::blockSignals)
        .def("inherits", &QObject::inherits)
        .def("isWidgetType", &QObject::isWidgetType)
        .def("killTimer", &QObject::killTimer)
        .def_property("objectName", &QObject::objectName, &QObject::setObjectName)
        .def_property_readonly("setObjectName", &QObject::setObjectName)
        .def_property("parent", &QObject::parent, &QObject::setParent, py::return_value_policy::reference)
        .def("setParent", &QObject::setParent)
        .def("deleteLater", &QObject::deleteLater)
        .def("startTimer", (int (QObject::*)(int, Qt::TimerType)) &QObject::startTimer)

        // deprecated
        .def("getObjectName", &QObject::objectName)
        .def("getParent", &QObject::parent, py::return_value_policy::reference)
        ;

    py::class_<QTimer>(m, "QTimer")
        .def_property("interval", &QTimer::interval, (void (QTimer::*)(int)) &QTimer::setInterval)
        .def("setInterval", (void (QTimer::*)(int)) &QTimer::setInterval)
        .def("isActive", &QTimer::isActive)
        .def("isSingleShot", &QTimer::isSingleShot)
        .def("setSingleShot", &QTimer::setSingleShot)
        .def_property_readonly("timerId", &QTimer::timerId)
        .def("start", (void (QTimer::*)()) &QTimer::start)
        .def("start", (void (QTimer::*)(int)) &QTimer::start)
        .def("stop", &QTimer::stop)
        .def_static("singleShot", (void(*)(int, const QObject*, const char*)) &QTimer::singleShot)

        // deprecated
        .def("getInterval", &QTimer::interval)
        .def("getTimerId", &QTimer::timerId)
        ;
}
