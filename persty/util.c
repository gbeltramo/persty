#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>
#include <math.h>

static PyObject*
get_A(PyObject* self, PyObject* args) {
    /* Parse arguments */
    PyObject* args_ptr;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &args_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Need list of arguments as input");
        return NULL;
    }

    /* Type Checks */
    PyObject* p_ptr = PyList_GetItem(args_ptr, 0);
    if (!PyList_Check(p_ptr)) {
        PyErr_SetString(PyExc_TypeError, "First argument needs to be list.");
        return NULL;
    }

    PyObject* q_ptr = PyList_GetItem(args_ptr, 1);
    if (!PyList_Check(q_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Second argument needs to be list.");
        return NULL;
    }

    PyObject* first_value_p = PyList_GetItem(p_ptr, 0);
    PyObject* first_value_q = PyList_GetItem(q_ptr, 0);
    if (! (PyFloat_Check(first_value_p) && PyFloat_Check(first_value_q)) ) {
        PyErr_SetString(PyExc_TypeError, "Elements of p and q must be floats");
        return NULL;
    }

    /* Max coordinate difference between p and q */
    Py_ssize_t d = PyList_Size(p_ptr);
    double p_0 = PyFloat_AsDouble(first_value_p);
    double q_0 = PyFloat_AsDouble(first_value_q);
    double delta = fabs(p_0 - q_0);
    double new_delta;
    Py_ssize_t coord_max_delta = 0;
    for (Py_ssize_t index = 0; index < d; ++index) {
        PyObject* p_i_ptr = PyList_GetItem(p_ptr, index);
        PyObject* q_i_ptr = PyList_GetItem(q_ptr, index);
        double p_i = PyFloat_AsDouble(p_i_ptr);
        double q_i = PyFloat_AsDouble(q_i_ptr);
        new_delta = fabs(p_i - q_i);
        if (new_delta > delta) {
            delta = new_delta;
            coord_max_delta = index;
        }
    }
    double radius = delta / 2.0;

    /* Create and return A as list of 2d float values */
    PyObject* A_ptr = PyList_New(0);
    for (Py_ssize_t index = 0; index < d; ++index) {
        PyObject* p_i_ptr = PyList_GetItem(p_ptr, index);
        PyObject* q_i_ptr = PyList_GetItem(q_ptr, index);
        double p_i = PyFloat_AsDouble(p_i_ptr);
        double q_i = PyFloat_AsDouble(q_i_ptr);

        if (index == coord_max_delta) {
            double mean_p_q = (p_i + q_i) / 2.0;
            PyObject* mean_p_q_ptr = PyFloat_FromDouble(mean_p_q);
            PyList_Append(A_ptr, mean_p_q_ptr);
            PyList_Append(A_ptr, mean_p_q_ptr);
        } else {
            if  (p_i <= q_i) {
                PyObject* low_ptr = PyFloat_FromDouble(q_i - radius);
                PyObject* high_ptr = PyFloat_FromDouble(p_i + radius);
                PyList_Append(A_ptr, low_ptr);
                PyList_Append(A_ptr, high_ptr);
            } else {
                PyObject* low_ptr = PyFloat_FromDouble(p_i - radius);
                PyObject* high_ptr = PyFloat_FromDouble(q_i + radius);
                PyList_Append(A_ptr, low_ptr);
                PyList_Append(A_ptr, high_ptr);
            }
        }
    }
    return A_ptr;
}

bool ball_and_rect_intersect(PyObject* rect_ptr, PyObject* y_ptr, double radius) {
    Py_ssize_t d = PyList_Size(y_ptr);

    for (Py_ssize_t index = 0; index < d; ++index) {
        PyObject* y_i_ptr    = PyList_GetItem(y_ptr, index);
        PyObject* low_i_ptr  = PyList_GetItem(rect_ptr, 2*index);
        PyObject* high_i_ptr = PyList_GetItem(rect_ptr, 2*index+1);
        double y_i    = PyFloat_AsDouble(y_i_ptr);
        double low_i  = PyFloat_AsDouble(low_i_ptr);
        double high_i = PyFloat_AsDouble(high_i_ptr);
        /* if y_i is not within low_i-r and high_i+r --> no intersection */
        if (!((low_i - radius < y_i) && (y_i < high_i + radius))) {
            return false;
        }
    }
    return true;
}

double* update_rect_get_new_rect(PyObject* rect_ptr, Py_ssize_t index,
    double y_i, double radius) {
    Py_ssize_t d = PyList_Size(rect_ptr) / 2;
    PyObject* y_i_plus_r_ptr     = PyFloat_FromDouble(y_i+radius);
    PyObject* y_i_minus_r_ptr    = PyFloat_FromDouble(y_i-radius);
    PyObject* low_i_ptr          = PyList_GetItem(rect_ptr, 2*index);
    PyObject* high_i_ptr         = PyList_GetItem(rect_ptr, 2*index+1);
    double low_i  = PyFloat_AsDouble(low_i_ptr);
    double high_i = PyFloat_AsDouble(high_i_ptr);

    double* new_rect = calloc(2*d, sizeof(double));
    PyObject* low_i_new_ptr;
    PyObject* high_i_new_ptr;
    double low_i_new;
    double high_i_new;

    if (low_i < y_i + radius && y_i + radius < high_i) {
        /* if y_i+r is in [low_i, high_i] new_rect range is [y_i+r, high_i]*/
        for (Py_ssize_t index_new_rect = 0; index_new_rect < d; ++index_new_rect) {
            low_i_new_ptr          = PyList_GetItem(rect_ptr, 2*index_new_rect);
            high_i_new_ptr         = PyList_GetItem(rect_ptr, 2*index_new_rect+1);
            low_i_new  = PyFloat_AsDouble(low_i_new_ptr);
            high_i_new = PyFloat_AsDouble(high_i_new_ptr);
            if (index_new_rect == index) {
                PyList_SetItem(rect_ptr, 2*index_new_rect+1, y_i_plus_r_ptr);
                new_rect[2*index_new_rect]   = y_i + radius;
                new_rect[2*index_new_rect+1] = high_i_new;
            } else {
                new_rect[2*index_new_rect]   = low_i_new;
                new_rect[2*index_new_rect+1] = high_i_new;
            }
        }
        return new_rect;
    } else if (low_i < y_i - radius && y_i - radius < high_i) {
        /* if y_i-r is in [low_i, high_i] new_rect range is [low_i, y_i-r]*/
        for (Py_ssize_t index_new_rect = 0; index_new_rect < d; ++index_new_rect) {
            low_i_new_ptr          = PyList_GetItem(rect_ptr, 2*index_new_rect);
            high_i_new_ptr         = PyList_GetItem(rect_ptr, 2*index_new_rect+1);
            low_i_new  = PyFloat_AsDouble(low_i_new_ptr);
            high_i_new = PyFloat_AsDouble(high_i_new_ptr);
            if (index_new_rect == index) {
                PyList_SetItem(rect_ptr, 2*index_new_rect, y_i_minus_r_ptr);
                new_rect[2*index_new_rect]   = low_i_new;
                new_rect[2*index_new_rect+1] = y_i - radius;
            } else {
                new_rect[2*index_new_rect]   = low_i_new;
                new_rect[2*index_new_rect+1] = high_i_new;
            }
        }
        return new_rect;
    } else {
        /* [y_i-r, y_i+r] covers [low_i, high_i] */
        return NULL;
    }

}

static PyObject*
update_list_hyperrectangles(PyObject* self, PyObject* args) {
    /* Parse arguments */
    PyObject* args_ptr;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &args_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Need list of arguments as input");
        return NULL;
    }

    PyObject* hyperrect_ptr = PyList_GetItem(args_ptr, 0);
    if (!PyList_Check(hyperrect_ptr)) {
        PyErr_SetString(PyExc_TypeError, "First argument needs to be list.");
        return NULL;
    }

    PyObject* y_ptr = PyList_GetItem(args_ptr, 1);
    if (!PyList_Check(hyperrect_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Second argument needs to be list.");
        return NULL;
    }

    PyObject* y_0_ptr = PyList_GetItem(y_ptr, 0);
    if (!PyFloat_Check(y_0_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Elements of point y must be floats.");
        return NULL;
    }

    PyObject* radius_ptr = PyList_GetItem(args_ptr, 2);
    double radius = PyFloat_AsDouble(radius_ptr);

    /* Update hyperrect_ptr */
    Py_ssize_t d = PyList_Size(y_ptr);
    PyObject* new_hyperrect_ptr = PyList_New(0);
    Py_ssize_t num_hyperrect = PyList_Size(hyperrect_ptr);

    PyObject* rect_ptr;
    double*   new_rect;
    PyObject* new_rect_ptr;

    for (Py_ssize_t index_rect = 0; index_rect < num_hyperrect; ++index_rect) {
        rect_ptr = PyList_GetItem(hyperrect_ptr, index_rect);
        if (ball_and_rect_intersect(rect_ptr, y_ptr, radius)) {
            for (Py_ssize_t index = 0; index < d; ++index) {
                PyObject* y_i_ptr    = PyList_GetItem(y_ptr, index);
                PyObject* low_i_ptr  = PyList_GetItem(rect_ptr, 2*index);
                PyObject* high_i_ptr = PyList_GetItem(rect_ptr, 2*index+1);
                double y_i    = PyFloat_AsDouble(y_i_ptr);
                double low_i  = PyFloat_AsDouble(low_i_ptr);
                double high_i = PyFloat_AsDouble(high_i_ptr);
                new_rect = update_rect_get_new_rect(rect_ptr,
                                                    index,
                                                    y_i,
                                                    radius);
                if (new_rect) {
                    new_rect_ptr = PyList_New(2*d);
                    for (Py_ssize_t index_new = 0; index_new < d; ++index_new) {
                        PyObject* low  = PyFloat_FromDouble(new_rect[2*index_new]);
                        PyObject* high = PyFloat_FromDouble(new_rect[2*index_new+1]);

                        PyList_SetItem(new_rect_ptr, 2*index_new, low);
                        PyList_SetItem(new_rect_ptr, 2*index_new+1, high);
                    }
                    PyList_Append(new_hyperrect_ptr, new_rect_ptr);
                }
                free(new_rect);
            }
        } else {
            PyList_Append(new_hyperrect_ptr, rect_ptr);
        }
    }
    return new_hyperrect_ptr;
}

bool check(PyObject* edges_ptr, PyObject* indices_edges_in_simplex_ptr,
           PyObject* simplex_ptr) {
    Py_ssize_t num_simplex_edges = PyList_Size(indices_edges_in_simplex_ptr);
    Py_ssize_t num_edges = PyList_Size(edges_ptr);

    PyObject* indices;
    Py_ssize_t i;
    Py_ssize_t j;
    long e1, e2, e3, e4;
    PyObject* edge;

    bool not_found = true;
    for (Py_ssize_t index = 0; index < num_simplex_edges; ++index) {
        not_found = true;
        indices = PyList_GetItem(indices_edges_in_simplex_ptr, index);
        i = PyLong_AsSsize_t(PyTuple_GetItem(indices, 0));
        j = PyLong_AsSsize_t(PyTuple_GetItem(indices, 1));

        e1 = PyLong_AsLong(PyTuple_GetItem(simplex_ptr, i));
        e2 = PyLong_AsLong(PyTuple_GetItem(simplex_ptr, j));

        for (Py_ssize_t index_edge = 0; index_edge < num_edges; ++index_edge) {
            edge = PyList_GetItem(edges_ptr, index_edge);
            e3 = PyLong_AsLong(PyList_GetItem(edge, 0));
            e4 = PyLong_AsLong(PyList_GetItem(edge, 1));
            if (e1 == e3 && e2 == e4) {
                not_found = false;
                break;
            }
        }
        if (not_found) {
            return false;
        }
    }
    return true;
}


static PyObject*
cliques(PyObject* self, PyObject* args) {
    /* Parse arguments */
    PyObject* args_ptr;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &args_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Argument passed needs to be a list");
        return NULL;
    }

    PyObject* edges_ptr = PyList_GetItem(args_ptr, 0);
    if (!PyList_Check(edges_ptr)) {
        PyErr_SetString(PyExc_TypeError, "First argument needs to be list.");
        return NULL;
    }

    PyObject* all_simplices_ptr = PyList_GetItem(args_ptr, 1);
    PyObject* indices_edges_in_simplex_ptr = PyList_GetItem(args_ptr, 2);
    PyObject* number_points_ptr = PyList_GetItem(args_ptr, 3);

    long number_points = PyLong_AsLong(number_points_ptr);
    Py_ssize_t number_simplices = PyList_Size(all_simplices_ptr);
    PyObject* simplices_ptr = PyList_New(0);
    PyObject* simplex_ptr;
    for (Py_ssize_t i = 0; i < number_simplices; ++i) {
        simplex_ptr = PyList_GetItem(all_simplices_ptr, i);
        if (check(edges_ptr, indices_edges_in_simplex_ptr, simplex_ptr)) {
            PyList_Append(simplices_ptr, simplex_ptr);
        }
    }

    return simplices_ptr;
}

// 2 TABLE OF METHODS TO EXPORT
PyMethodDef method_table[] = {
    {"get_A",
     (PyCFunction) get_A,
     METH_VARARGS,
     "Find A"
     "\n"
    },
    {"update_list_hyperrectangles",
     (PyCFunction) update_list_hyperrectangles,
     METH_VARARGS,
     "Find new list of hyperrectangles"
     "\n"
    },
    {"cliques",
     (PyCFunction) cliques,
     METH_VARARGS,
     "Find d dimensional cliques on given edges."
     "\n"
    },
	{NULL, NULL, 0, NULL} // end of table
};

// 3 STRUCT DEFINING MODULE
PyModuleDef util_module = {
			      PyModuleDef_HEAD_INIT,
			      "util",             // name of module
			      "Functions to speed up Delaunay edges computations.",
			      -1,
			      method_table,
			      NULL, NULL,
			      NULL, NULL
};

// 4 INIT FUNC
PyMODINIT_FUNC PyInit_util(void)   // PyInit_<NAME_OF_MODULE>
{
  return PyModule_Create(&util_module);
}
