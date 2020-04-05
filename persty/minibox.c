#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>

// 1 METHODS
bool y_inside_minibox(double* mini_pq, double* y, Py_ssize_t d) {
    double y_i = 0.0;
    for (Py_ssize_t i = 0; i < d; ++i) {
        y_i = y[i];
        if (y_i <= mini_pq[2*i] || y_i >= mini_pq[2*i+1]) {
            return false;
        }
    }
    return true;
}

static PyObject*
edges(PyObject* self, PyObject* args) {
    /* Parse arguments */
    PyObject* points_pyptr;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &points_pyptr)) {
        PyErr_SetString(PyExc_TypeError, "Argument passed needs to be a list");
        return NULL;
    }

    Py_ssize_t n = PyList_Size(points_pyptr);
    PyObject* first_item_pyptr = PyList_GetItem(points_pyptr, 0);
    if(!PyList_Check(first_item_pyptr)) {
        PyErr_SetString(PyExc_TypeError, "List items must be lists.");
        return NULL;
    }
    Py_ssize_t d = PyList_Size(first_item_pyptr);

    /* Read points into dynamically allocated array */
    double* array_points = calloc(n*d, sizeof(double));
    PyObject*  tmp_item_pyptr;
    for (Py_ssize_t index = 0; index < n; ++index) {
        tmp_item_pyptr = PyList_GetItem(points_pyptr, index);
        Py_ssize_t tmp_d = PyList_Size(tmp_item_pyptr);
        if(!PyList_Check(tmp_item_pyptr)) {
            PyErr_SetString(PyExc_TypeError, "List items must be lists.");
            return NULL;
        }
        if(tmp_d != d) {
            PyErr_SetString(PyExc_TypeError, "Sublist must contain d elements.");
            return NULL;
        }
        for (Py_ssize_t i = 0; i < d; ++i) {
            array_points[index*d+i] = PyFloat_AsDouble(PyList_GetItem(tmp_item_pyptr, i));
        }
    }

    /* Search Minibox edges */
    PyObject* edges_pyptr = PyList_New(0);         // empty output list
    PyObject* e_pyptr = PyTuple_New(2);

    double* p;
    double* q;
    double* y;
    double* mini_pq = calloc(2*d, sizeof(double));
    bool add_edge = true;

    for (Py_ssize_t p_ind = 0; p_ind < n; ++p_ind) {
        for (Py_ssize_t q_ind = p_ind+1; q_ind < n; ++q_ind) {
            e_pyptr = PyTuple_New(2);               // avoid aliasing
            PyTuple_SetItem(e_pyptr, 0, PyLong_FromSsize_t(p_ind));
            PyTuple_SetItem(e_pyptr, 1, PyLong_FromSsize_t(q_ind));

            /* init p and q */
            p = (array_points + p_ind*d);
            q = (array_points + q_ind*d);

            /* init mini_pq */
            for (Py_ssize_t i = 0; i < d; ++i) {
                double p_i = p[i];
                double q_i = q[i];
                if (p_i <= q_i) {
                    mini_pq[2*i]   = p_i;
                    mini_pq[2*i+1] = q_i;
                } else {
                    mini_pq[2*i]   = q_i;
                    mini_pq[2*i+1] = p_i;
                }
            }

            add_edge = true;
            for (Py_ssize_t y_ind = 0; y_ind < n; ++y_ind) {
                if (y_ind == p_ind || y_ind == q_ind) {
                    continue;
                }
                y = (array_points + y_ind*d);
                if(y_inside_minibox(mini_pq, y, d)) {
                    add_edge = false;
                    break;
                }
            }
            if (add_edge) {
                PyList_Append(edges_pyptr, e_pyptr);
            }
        }
    }

    free(array_points);
    free(mini_pq);
    return edges_pyptr;
}

// 2 TABLE OF METHODS TO EXPORT
PyMethodDef method_table[] = {
                  {"edges",
                   (PyCFunction) edges,
                   METH_VARARGS,
                   "Minibox edges on d-dimensional points"
                   "\n"
                   "\nFind the Minibox edges iterating on all possible"
                   "\npairs of indices in `points`."
                   "\n"
                   "\nParameters"
                   "\n----------"
                   "\npoints: list of `n` lists containing `d` floats each"
                   "\n\tThe list of d-dimensional points."
                   "\n"
                   "\nReturn"
                   "\n------"
                   "\nminibox_edges: list of pairs of integers"
                   "\n\tThe indices of elements in `points` forming a"
                   "\n\tMinibox edge."
                   "\n "
                   "\n"},
			      {NULL, NULL, 0, NULL} // end of table
};

// 3 STRUCT DEFINING MODULE
PyModuleDef minibox_module = {
			      PyModuleDef_HEAD_INIT,
			      "minibox",             // name of module
			      "Minibox edges C extension",
			      -1,
			      method_table,
			      NULL, NULL,
			      NULL, NULL
};

// 4 INIT FUNC
PyMODINIT_FUNC PyInit_minibox(void)   // PyInit_<NAME_OF_MODULE>
{
  return PyModule_Create(&minibox_module);
}
