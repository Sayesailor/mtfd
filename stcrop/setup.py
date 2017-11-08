# -*- encoding:utf8 -*-

try:
    from setuptools import setup, find_packages, Extension
except ImportError:
    from distutils.core import setup, find_packages
import sys

if sys.version_info.major == 2:
    print('python 2.x is unsupported, please update to python 3.5+')
    pymtfd = './facecrop/libmtfd.so'
else:
    pymtfd = './facecrop/libmtfd.so'

setup(
    name='facecrop',
    version='1.0',
    author='SeetaTech',
    author_email='yizhong.shen@seetatech.com',
    package_data={'': ['*.*']},
    packages=find_packages(),
    data_files=[('facecrop/', [pymtfd])],
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python',
        'Operating System :: Unix',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 2.7',
    ],
)
