if [[ $1 == "clean" ]]; then
    rm build -rf
    exit 0
fi

mkdir -p build
python setup.py install
