#include <pybind11/stl.h>
#include <sonata/Provider.hpp>

namespace py11 = pybind11;
using namespace pybind11::literals;
using namespace sonata;

struct margo_instance;
typedef struct margo_instance* margo_instance_id;
typedef py11::capsule pymargo_instance_id;

#define MID2CAPSULE(__mid)   py11::capsule((void*)(__mid), "margo_instance_id", nullptr)
#define CAPSULE2MID(__caps)  (margo_instance_id)(__caps)

PYBIND11_MODULE(_sonata_server, m) {
    m.doc() = "Sonata server C++ extension";

    py11::class_<Provider>(m, "SonataProvider")
        .def(py11::init<pymargo_instance_id, uint16_t>())
        .def("set_security_token", &Provider::setSecurityToken,
                "Set the security token of the provider.",
                "token"_a);
}
