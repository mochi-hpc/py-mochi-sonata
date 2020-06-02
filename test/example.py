import sys
sys.path.append('../build/lib.linux-x86_64-3.7')
import json

import pymargo
from pymargo.core import Engine

from pysonata.provider import SonataProvider
from pysonata.client import SonataClient
from pysonata.admin import SonataAdmin

def test(engine):
    address = str(engine.addr())
    admin = SonataAdmin(engine)
    client = SonataClient(engine)
    # create a database
    admin.create_database(address, 0, 'mydb', 'unqlite', '{ "path" : "mydb" }')
    # get the database
    database = client.open(address, 0, 'mydb')
    # create a collection
    database.create('mycollection')
    # open an existing collection
    collection = database.open('mycollection')
    # store a record
    record = { 'name' : 'Matthieu', 'age' : 32 }
    record_id = collection.store(json.dumps(record))
    print(record_id)
    # fetch a record
    fetched_record = json.loads(collection.fetch(record_id))
    print(fetched_record)
    # store multiple records
    records = [ json.dumps(x) for x in [
            { 'name' : 'Lucas', 'age' : 29 },
            { 'name' : 'Vincent', 'age' : 27 },
            { 'name' : 'Andreea', 'age' : 31 },
        ]
    ]
    record_ids = collection.store_multi(records)
    print(record_ids)
    # fetch multiple records
    fetched_records = [ json.loads(x) for x in collection.fetch_multi(record_ids) ]
    print(fetched_records)
    # get all the records
    all_records = [ json.loads(x) for x in collection.all ]
    print(all_records)
    # get the size of the collection
    print(collection.size)
    # get the last record id in the collection
    print(collection.last_record_id)
    # update record 1 (Lucas)
    collection.update(1, json.dumps({ "name" : "Lucas", "age" : 30 }))
    print(json.loads(collection.fetch(1)))
    # find out all the records where age > 30
    jx9_filter = "function($user) { return $user.age > 30; }"
    filtered_records = [ json.loads(x) for x in collection.filter(jx9_filter) ]
    print(filtered_records)
    # destroy database
    admin.destroy_database(address, 0, 'mydb')

if __name__ == '__main__':
    with Engine('na+sm', pymargo.server) as engine:
        provider = SonataProvider(engine, 0)
        test(engine)
        del provider
        engine.finalize()
