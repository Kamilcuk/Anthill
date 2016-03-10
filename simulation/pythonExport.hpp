/*
 * pythonExport.hpp
 *
 *  Created on: 10 mar 2016
 *      Author: ms
 */

#include <boost/python.hpp>
namespace py = boost::python;

template<class T>
py::list std_vector_to_py_list(const std::vector<T>& v)
{
    py::object get_iter = py::iterator<std::vector<T> >();
    py::object iter = get_iter(v);
    py::list l(iter);
    return l;
}
