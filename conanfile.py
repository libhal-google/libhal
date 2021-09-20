from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake
from conan.tools.layout import cmake_layout


class LibEmbeddedHalConan(ConanFile):
    name = "libembeddedhal"
    version = "0.0.1"
    license = "Apache License Version 2.0"
    author = "Khalil Estell"
    url = "https://github.com/SJSU-Dev2/libembeddedhal"
    description = "A collection of interfaces and abstractions for embedded peripherals and devices using modern C++"
    topics = ("peripherals", "hardware")
    exports_sources = "CMakeLists.txt", "libembeddedhal/*"
    no_copy_source = True
    generators = "cmake_find_package"

    def config_options(self):
        pass

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def package_info(self):
        self.cpp_info.includedirs = ["."]

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
