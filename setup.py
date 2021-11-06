from distutils.core import setup
from distutils.extension import Extension

mini_image_lib = Extension(
    'mini_image_lib',
    sources=['./mini_img_lib/mt.cpp', './mini_img_lib/py_functions.cpp', 
    './mini_img_lib/blur.cpp', './mini_img_lib/edge_det.cpp', 
    './mini_img_lib/kmean.cpp', './mini_img_lib/gray.cpp'],
    libraries=['boost_python39-mt'],
    extra_compile_args=['-std=c++17', '-lboost_python39-mt']
)

setup(
    name='hello-world',
    version='0.1',
    ext_modules=[mini_image_lib])


    