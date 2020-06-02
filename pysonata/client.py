import _sonata_client


class SonataClient(_sonata_client.SonataClient):

    def __init__(self, engine):
        super().__init__(engine.get_internal_mid())
