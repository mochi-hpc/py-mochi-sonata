from distutils.core import setup
from distutils.extension import Extension
from distutils.sysconfig import get_config_vars
from distutils.command.build_clib import build_clib
from distutils.command.build_ext import build_ext
import pybind11
import pkgconfig
import os
import os.path
import sys

(opt,) = get_config_vars('OPT')
os.environ['OPT'] = " ".join(flag for flag in opt.split() if flag != '-Wstrict-prototypes')

sonata_client = pkgconfig.parse('sonata-client')
sonata_server = pkgconfig.parse('sonata-server')
sonata_admin  = pkgconfig.parse('sonata-admin')

extra_compile_args=['-std=c++14']
if sys.platform == 'darwin':
    extra_compile_args.append('-mmacosx-version-min=10.9')

sonata_server_ext = Extension('_sonata_server',
        ['pysonata/src/server_module.cpp'],
        libraries=sonata_server['libraries'],
        library_dirs=sonata_server['library_dirs'],
        include_dirs=sonata_server['include_dirs'] + [pybind11.get_include()],
        language='c++14',
        extra_compile_args=extra_compile_args,
        depends=[])

sonata_admin_ext = Extension('_sonata_admin',
        ['pysonata/src/admin_module.cpp'],
        libraries=sonata_admin['libraries'],
        library_dirs=sonata_admin['library_dirs'],
        include_dirs=sonata_admin['include_dirs'] + [pybind11.get_include()],
        language='c++14',
        extra_compile_args=extra_compile_args,
        depends=[])

sonata_client_ext = Extension('_sonata_client',
        ['pysonata/src/client_module.cpp'],
        libraries=sonata_client['libraries'],
        library_dirs=sonata_client['library_dirs'],
        include_dirs=sonata_client['include_dirs'] + [pybind11.get_include()],
        language='c++14',
        extra_compile_args=extra_compile_args,
        depends=[])

setup(name='py-sonata',
      version='0.1',
      author='Matthieu Dorier',
      description='''Python interface for the Sonata Mochi service''',
      ext_modules=[ sonata_client_ext,
                    sonata_server_ext,
                    sonata_admin_ext,
                  ],
      packages=['pysonata']
    )
