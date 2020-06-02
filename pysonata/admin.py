import _sonata_admin


class SonataAdmin(_sonata_admin.SonataAdmin):

    def __init__(self, engine):
        super().__init__(engine.get_internal_mid())
