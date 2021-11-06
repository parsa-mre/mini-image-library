#include "./mini_img_lib.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
namespace py = boost::python;
namespace numpy = boost::python::numpy;

int NUMBER_OF_THREADS = 2; // number of threads
int H, W, k;               // height, width, kernel_size/2
int NUMBER_OF_CLUSTERS = 8;

std::vector<float> blur_kernel;

std::vector<std::vector<std::vector<float> > > image, blured_image, _cache_3_c; // three channels
std::vector<std::vector<float> > gray_image, edge_image, _cache_1_c, clusters;  // two channels
std::vector<std::vector<int> > clustered_image;

void convert_to_numpy_1C(const std::vector<std::vector<float> > &input, py::object obj)
{
    PyObject *pobj = obj.ptr();
    Py_buffer pybuf;
    PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE);
    void *buf = pybuf.buf;
    double *p = (double *)buf;
    Py_XDECREF(pobj);

    u_int n_rows = input.size();
    u_int n_cols = input[0].size();

    for (u_int i = 0; i < n_rows; i++)
    {
        for (u_int j = 0; j < n_cols; j++)
        {
            p[i * n_cols + j] = double(input[i][j]);
        }
    }
}

void convert_to_numpy_1C_i(const std::vector<std::vector<int> > &input, py::object obj)
{
    PyObject *pobj = obj.ptr();
    Py_buffer pybuf;
    PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE);
    void *buf = pybuf.buf;
    double *p = (double *)buf;
    Py_XDECREF(pobj);

    u_int n_rows = input.size();
    u_int n_cols = input[0].size();

    for (u_int i = 0; i < n_rows; i++)
    {
        for (u_int j = 0; j < n_cols; j++)
        {
            p[i * n_cols + j] = double(input[i][j]);
        }
    }
}

void convert_to_numpy_2C(const std::vector<std::vector<std::vector<float> > > &input, py::object obj)
{
    PyObject *pobj = obj.ptr();
    Py_buffer pybuf;
    PyObject_GetBuffer(pobj, &pybuf, PyBUF_SIMPLE);
    void *buf = pybuf.buf;
    double *p = (double *)buf;
    Py_XDECREF(pobj);

    u_int n_rows = input.size();
    u_int n_cols = input[0].size();

    for (u_int i = 0; i < n_rows; i++)
    {
        for (u_int j = 0; j < n_cols; j++)
        {
            p[3 * i * n_cols + 3 * j + 0] = double(input[i][j][0]);
            p[3 * i * n_cols + 3 * j + 1] = double(input[i][j][1]);
            p[3 * i * n_cols + 3 * j + 2] = double(input[i][j][2]);
        }
    }
}

void use(const py::list &I, py::object h, py::object w, py::object number_of_threads)
{
    NUMBER_OF_THREADS = py::extract<int>(number_of_threads);
    W = py::extract<int>(w);
    H = py::extract<int>(h);
    boost::python::ssize_t N = py::len(I);
    assert(N == W * H * 3);
    int index = 0;
    image = std::vector<std::vector<std::vector<float> > >(H, std::vector<std::vector<float> >(W, std::vector<float>(3, 0)));
    for (int i = 0; i < H; ++i)
    {
        for (int j = 0; j < W; ++j)
        {
            image[i][j][0] = py::extract<int>(I[index++]);
            image[i][j][1] = py::extract<int>(I[index++]);
            image[i][j][2] = py::extract<int>(I[index++]);
        }
    }
}

void boost_kmean(py::object obj, py::object n_c, py::object itt)
{
    kmeans(py::extract<int>(n_c), py::extract<int>(itt));
    convert_to_numpy_1C_i(clustered_image, obj);
}

void boost_blur(py::object obj, py::object i, py::object f_s, py::object std)
{
    blur(py::extract<int>(i), py::extract<int>(f_s), py::extract<float>(std));
    convert_to_numpy_2C(blured_image, obj);
}

void boost_gray(py::object obj)
{
    gray();
    convert_to_numpy_1C(gray_image, obj);
}

void boost_edge(py::object obj)
{
    edge_detection();
    convert_to_numpy_1C(edge_image, obj);
}

BOOST_PYTHON_MODULE(mini_image_lib)
{
    using namespace boost::python;
    def("use", use);
    def("blur", boost_blur);
    def("gray_scale", boost_gray);
    def("detect_edge", boost_edge);
    def("kmean", boost_kmean);
}
