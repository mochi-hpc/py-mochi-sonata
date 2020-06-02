#include <pybind11/stl.h>
#include <sonata/Client.hpp>

namespace py11 = pybind11;
using namespace pybind11::literals;
using namespace sonata;

struct margo_instance;
typedef struct margo_instance* margo_instance_id;
typedef py11::capsule pymargo_instance_id;

#define MID2CAPSULE(__mid)   py11::capsule((void*)(__mid), "margo_instance_id", nullptr)
#define CAPSULE2MID(__caps)  (margo_instance_id)(__caps)

PYBIND11_MODULE(_sonata_client, m) {
    m.doc() = "Sonata client C++ extension";
    py11::class_<Client>(m, "SonataClient")
        .def(py11::init<pymargo_instance_id>())
        .def("open", &Client::open,
                "Open a remote database."
                "address"_a,
                "provider_id"_a,
                "database_name"_a,
                "check"_a=true);
    py11::class_<Database>(m, "SonataDatabase")
        .def("exists", &Database::exists, "Check if a collection exists.",
                "collection_name"_a)
        .def("create", &Database::create, "Create a collection.",
                "collection_name"_a)
        .def("open", &Database::open, "Open a collection from the database.",
                "collection_name"_a,
                "check"_a=true)
        .def("drop", &Database::drop, "Drop a collection from the database.",
                "collection_name"_a)
        .def("execute", [](const Database& db,
                           const std::string& code,
                           const std::unordered_set<std::string>& variables,
                           bool commit) {
                    std::unordered_map<std::string,std::string> result;
                    db.execute(code, variables, &result, commit);
                    return result;
                }, "Execute code on the database.",
                "code"_a,
                "variables"_a = py11::list(),
                "commit"_a = false)
        .def("commit", &Database::commit, "Commit changes to the database's persistent storage.");
    py11::class_<Collection>(m, "SonataCollection")
        .def("store", (uint64_t (Collection::*)(const std::string&, bool) const) &Collection::store,
                "Store a record into the collection.",
                "record"_a, "commit"_a=false)
        .def("store_multi", [](const Collection& coll,
                               const std::vector<std::string>& records,
                               bool commit) {
                    std::vector<uint64_t> ids(records.size());
                    coll.store_multi(records, ids.data(), commit);
                    return ids;
                }, "Store multiple records into the collection.",
                "records"_a, "commit"_a=false)
        .def("fetch", [](const Collection& coll, uint64_t id) {
                    std::string result;
                    coll.fetch(id, &result);
                    return result;
                }, "Fetch a record from the collection.",
                "id"_a)
        .def("fetch_multi", [](const Collection& coll, const std::vector<uint64_t>& ids) {
                    std::vector<std::string> result;
                    coll.fetch_multi(ids.data(), ids.size(), &result);
                    return result;
                }, "Fetch multiple records from the collection.",
                "ids"_a)
        .def("filter", [](const Collection& coll, const std::string& code) {
                    std::vector<std::string> result;
                    coll.filter(code, &result);
                    return result;
                }, "Fetches records satisfiying the provided jx9 condition.",
                "code"_a)
        .def("update", [](const Collection& coll,
                          uint64_t id,
                          const std::string& record,
                          bool commit) {
                    coll.update(id, record, commit);
                },
                "Update a record in the collection.",
                "id"_a, "record"_a, "commit"_a=false)
        .def("update_multi", [](const Collection& coll,
                                const std::vector<uint64_t>& ids,
                                const std::vector<std::string>& records,
                                bool commit) {
                    std::vector<bool> result;
                    coll.update_multi(ids.data(), records, &result, commit);
                    return result;
                }, "Update multipl records in the collection."
                "ids"_a, "records"_a, "commit"_a=false)
        .def_property_readonly("all", [](const Collection& coll) {
                    std::vector<std::string> result;
                    coll.all(&result);
                    return result;
                }, "Get all the records from the collection.")
        .def_property_readonly("last_record_id", &Collection::last_record_id,
                "Get the last record id of the collection.")
        .def_property_readonly("size", &Collection::size,
                "Get the size of the collection.")
        .def("erase", (void (Collection::*)(uint64_t, bool)) &Collection::erase,
                "Erase a particular record from the collection.",
                "id"_a, "commit"_a=false)
        .def("erase_multi", [](const Collection& coll, const std::vector<uint64_t>& ids, bool commit) {
                    coll.erase_multi(ids.data(), ids.size(), commit);
                }, "Erase multiple records from the collection.",
                "ids"_a, "commit"_a=false);
}
