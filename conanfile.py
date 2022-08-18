from conans import ConanFile, CMake, tools


class libhal_conan(ConanFile):
    name = "libhal"
    version = "0.0.1"
    license = "Apache License Version 2.0"
    author = "Khalil Estell"
    url = "https://github.com/libhal/libhal"
    description = "A collection of interfaces and abstractions for embedded peripherals and devices using modern C++"
    topics = ("peripherals", "hardware", "abstraction", "devices")
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*.h")
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
