import os
import re
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeBuild(build_ext):
    def build_extension(self, ext: Extension) -> None:
        base_dir = Path(__file__).parent
        ext_dir = (base_dir / self.get_ext_fullpath(ext.name)).parent

        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={ext_dir}",
            f"-DCMAKE_BUILD_TYPE=Debug",
        ]
        build_args = []

        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            if hasattr(self, "parallel") and self.parallel:
                build_args += [f"-j{self.parallel}"]

        build_dir = base_dir / "build"
        if not build_dir.exists():
            build_dir.mkdir(parents=True)
        print("cmd:", " ".join(["cmake", base_dir.as_posix(), *cmake_args]))
        print("cmd:", " ".join(["cmake", "--build", ".", *build_args]))

        subprocess.run(["cmake", base_dir, *cmake_args], cwd=build_dir, check=True)
        subprocess.run(
            ["cmake", "--build", ".", *build_args], cwd=build_dir, check=True
        )


setup(
    name="pystl",
    version="0.0.1",
    author="Axiomofchoice-hjt",
    description="Description",
    long_description="",
    ext_modules=[Extension("pystl", [])],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={"test": ["pytest>=6.0"]},
    python_requires=">=3.7",
    package_data={
        "pystl": ["py.typed"],
    },
)
