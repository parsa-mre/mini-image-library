python setup.py build_ext --inplace
rm mini_image_lib.so
mv mini_image_lib.cpython-39-darwin.so mini_image_lib.so
python example.py
# TIMEFMT=$'real\t%E\nuser\t%U\nsys\t%S'