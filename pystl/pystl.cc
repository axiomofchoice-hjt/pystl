#include <pybind11/pybind11.h>

#include "TreeDict.h"

PYBIND11_MODULE(pystl, m) {
    m.doc() = "pystl";

    TreeDict::bind(m);
}
