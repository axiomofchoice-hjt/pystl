pip uninstall pystl -y

if [[ $1 == "clean" ]]; then
    rm build -rf
    exit 0
fi

mkdir -p build
pip install .
pybind11-stubgen pystl -o pystl
