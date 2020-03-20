#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>

// 1 METHODS
double* minibox_array(double* p_ptr, double* q_ptr, size_t d) {
    double* mini_pq = calloc(2*d, sizeof(double));
    for (size_t i = 0; i < d; ++i) {
        double p_i = p_ptr[i];
        double q_i = q_ptr[i];
        if (p_i <= q_i) {
            mini_pq[2*i]   = p_i;
            mini_pq[2*i+1] = q_i;
        } else {
            mini_pq[2*i]   = q_i;
            mini_pq[2*i+1] = p_i;
        }
    }
    return mini_pq;
}

bool y_inside_minibox(double* mini_pq, double* y_ptr, size_t d) {
    for (size_t i = 0; i < d; ++i) {
        if (y_ptr[i] <= mini_pq[2*i] || y_ptr[i] >= mini_pq[2*i+1]) {
            return false;
        }
    }
    return true;
}

static PyObject*
edges(PyObject* self, PyObject* args) {
    /* Parse arguments */
    PyObject* points_ptr;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &points_ptr)) {
        PyErr_SetString(PyExc_TypeError, "Argument passed needs to be a list");
        return NULL;
    }

    Py_ssize_t n = PyList_Size(points_ptr);
    PyObject* first_item_ptr = PyList_GetItem(points_ptr, 0);
    Py_ssize_t d = PyList_Size(first_item_ptr);

    /* Read points into dynamically allocated array */
    double* array_ptr = calloc(n*d, sizeof(double));

    for (Py_ssize_t index = 0; index < n; ++index) {
        PyObject* tmp_item_ptr = PyList_GetItem(points_ptr, index);
        Py_ssize_t tmp_d = PyList_Size(tmp_item_ptr);
        if(!PyList_Check(tmp_item_ptr)) {
            PyErr_SetString(PyExc_TypeError, "List items must be lists.");
            return NULL;
        }
        if(tmp_d != d) {
            PyErr_SetString(PyExc_TypeError, "Sublist must contain d elements.");
            return NULL;
        }
        for (Py_ssize_t coord = 0; coord < d; ++coord) {
            PyObject* value_ptr = PyList_GetItem(tmp_item_ptr, coord);
            double value = PyFloat_AsDouble(value_ptr);
            array_ptr[index*d+coord] = value;
        }
    }

    /* Search Minibox edges */
    PyObject* edges_ptr = PyList_New(0);         // empty output list
    PyObject* e_ptr = PyTuple_New(2);
    double* p_ptr;
    double* q_ptr;
    double* mini_pq;
    double* y_ptr;
    bool add_edge;

    for (size_t first_ind = 0; first_ind < n; ++first_ind) {
        for (size_t second_ind = first_ind+1; second_ind < n; ++second_ind) {
            e_ptr = PyTuple_New(2);               // avoid aliasing
            PyObject* value0_ptr = PyLong_FromSize_t(first_ind);
            PyObject* value1_ptr = PyLong_FromSize_t(second_ind);
            PyTuple_SetItem(e_ptr, 0, value0_ptr);
            PyTuple_SetItem(e_ptr, 1, value1_ptr);

            p_ptr = (array_ptr + first_ind*d);
            q_ptr = (array_ptr + second_ind*d);
            mini_pq = minibox_array(p_ptr, q_ptr, d);
            add_edge = true;

            for (size_t y_ind = 0; y_ind < n; ++y_ind) {
                if (y_ind == first_ind || y_ind == second_ind) {
                    continue;
                }
                y_ptr = (array_ptr + y_ind*d);
                if(y_inside_minibox(mini_pq, y_ptr, d)) {
                    add_edge = false;
                    break;
                }
            }
            if (add_edge) {
                PyList_Append(edges_ptr, e_ptr);
            }
        }
    }
    free(array_ptr);
    free(mini_pq);

    return edges_ptr;
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
