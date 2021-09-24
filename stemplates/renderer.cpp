#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <iostream>
#include <string>

using u64 = unsigned long long;

static PyObject* render(PyObject* self, PyObject* args, PyObject* kwargs) {
  const char* text;
  if (!PyArg_ParseTuple(args, "s", &text)) {
    return NULL;
  }
  u64 text_len = strlen(text);
  u64 pos = 0;
  auto* res_s = new std::string("");
  auto next_char = [&]() -> char {
    if (pos < text_len) {
      char ch = text[pos];
      ++pos;
      return ch;
    }
    return 0;
  };
  auto curr_char = [&]() -> char {
    if (pos < text_len) {
      return text[pos];
    }
    return 0;
  };
  auto peek_char = [&]() -> char {
    u64 npos = pos + 1;
    if (npos < text_len) {
      return text[npos];
    }
    return 0;
  };
  PyObject* builtins = PyEval_GetBuiltins();
  PyObject* eval = PyDict_GetItemString(builtins, "eval");
  while (pos < text_len) {
    if (curr_char() == '\\' && peek_char() == '{') {
      // Handle escaped sequences
      next_char();  // skip the backslash
      *res_s += next_char();
    } else if (curr_char() == '{' && peek_char() == '%') {
      next_char();
      next_char();
      std::string expr = "";
      while (pos < text_len && !(curr_char() == '%' && peek_char() == '}')) {
        expr += curr_char();
        ++pos;
      }
      // Evaluate expression using Python built-in eval()
      PyObject* expr_o = PyUnicode_FromString(expr.c_str());
      PyObject* res = PyObject_CallFunction(eval, "(OO)", expr_o, kwargs);
      PyObject* repr = PyObject_Str(res);
      PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
      const char* e = PyBytes_AS_STRING(str);
      res_s->append(e);
      next_char();
      next_char();
    } else {
      *res_s += text[pos];
      ++pos;
    }
  }
  PyObject* res = PyUnicode_FromString(res_s->c_str());
  return res;
}

static PyMethodDef tRendererMethods[] = {
    {"render", (PyCFunction)render, METH_VARARGS | METH_KEYWORDS,
     "Render template"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef tRendererModule = {
    PyModuleDef_HEAD_INIT, "renderer", /* name of module */
    NULL,                              /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
    tRendererMethods};

PyMODINIT_FUNC PyInit_renderer(void) {
  return PyModule_Create(&tRendererModule);
}

int main(int argc, char* argv[]) {
  wchar_t* program = Py_DecodeLocale(argv[0], NULL);
  if (program == NULL) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    exit(1);
  }

  /* Add a built-in module, before Py_Initialize */
  if (PyImport_AppendInittab("renderer", PyInit_renderer) == -1) {
    fprintf(stderr, "Error: could not extend in-built modules table\n");
    exit(1);
  }

  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(program);

  /* Initialize the Python interpreter.  Required.
     If this step fails, it will be a fatal error. */
  Py_Initialize();

  /* Optionally import the module; alternatively,
     import can be deferred until the embedded script
     imports it. */
  PyObject* pmodule = PyImport_ImportModule("renderer");
  if (!pmodule) {
    PyErr_Print();
    fprintf(stderr, "Error: could not import module 'spam'\n");
  }

  PyMem_RawFree(program);
  return 0;
}

