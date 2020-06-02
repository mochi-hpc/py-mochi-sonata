#include <pybind11/stl.h>
#include <sonata/Admin.hpp>

namespace py11 = pybind11;
using namespace pybind11::literals;
using namespace sonata;

struct margo_instance;
typedef struct margo_instance* margo_instance_id;
typedef py11::capsule pymargo_instance_id;

#define MID2CAPSULE(__mid)   py11::capsule((void*)(__mid), "margo_instance_id", nullptr)
#define CAPSULE2MID(__caps)  (margo_instance_id)(__caps)

PYBIND11_MODULE(_sonata_admin, m) {
    m.doc() = "Sonata admin C++ extension";

    py11::class_<Admin>(m, "SonataAdmin")
        .def(py11::init<pymargo_instance_id>())
        .def("create_database", [](const Admin& admin,
                                   const std::string& address,
                                   uint16_t provider_id,
                                   const std::string& name,
                                   const std::string& type,
                                   const std::string& config,
                                   const std::string& token) {
                    admin.createDatabase(address, provider_id, name, type, config, token);
                },
                "address"_a,
                "provider_id"_a,
                "name"_a,
                "type"_a,
                "config"_a,
                "security_token"_a=std::string(""))
        .def("attach_database", [](const Admin& admin,
                                   const std::string& address,
                                   uint16_t provider_id,
                                   const std::string& name,
                                   const std::string& type,
                                   const std::string& config,
                                   const std::string& token) {
                    admin.attachDatabase(address, provider_id, name, type, config, token);
                },
                "address"_a,
                "provider_id"_a,
                "name"_a,
                "type"_a,
                "config"_a,
                "security_token"_a=std::string(""))
        .def("detach_database", [](const Admin& admin,
                                   const std::string& address,
                                   uint16_t provider_id,
                                   const std::string& name,
                                   const std::string& token) {
                    admin.detachDatabase(address, provider_id, name, token);
                },
                "address"_a,
                "provider_id"_a,
                "name"_a,
                "security_token"_a=std::string(""))
        .def("destroy_database", [](const Admin& admin,
                                    const std::string& address,
                                    uint16_t provider_id,
                                    const std::string& name,
                                    const std::string& token) {
                    admin.destroyDatabase(address, provider_id, name, token);
                },
                "address"_a,
                "provider_id"_a,
                "name"_a,
                "security_token"_a=std::string(""))
        ;
}
