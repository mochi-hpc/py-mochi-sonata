import _sonata_server


class SonataProvider(_sonata_server.SonataProvider):

    def __init__(self, engine, provider_id=0):
        super().__init__(engine.get_internal_mid(), provider_id)
