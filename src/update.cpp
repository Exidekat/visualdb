/* Update logic and stats from py/lua */
#include "visualdb.h"
#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif

PyObject* pName, * pModule, * pDict, * pFunc, * pValue;
std::string speech;

void pyListen()
{
    int argc = 4;
    const char* pyListen = "listen"; //.py

    if (argc < 3)
    {
        printf("Usage: exe_name python_source function_name\n");
        exit(1);
    }

    // Initialize the Python Interpreter
    Py_Initialize();
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));

    // Build the name object
    pName = PyUnicode_FromString(pyListen);

    // Load the module object
    pModule = PyImport_Import(pName);
    if (!pModule)
    {
        PyErr_Print();
        printf("ERROR in pModule\n");
        exit(1);
    }

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, "pullSpeech");

    if (PyCallable_Check(pFunc))
    {
        PyObject_CallObject(pFunc, NULL);
    }
    else
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();
}

void update() 
{
	//std::cout << PyObject_CallFunction(pPullSpeech, NULL) << std::endl;
}