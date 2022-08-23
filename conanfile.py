from conans import ConanFile


class libhal_conan(ConanFile):
    name = "libhal"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://libhal.github.io/libhal"
    description = ("A collection of interfaces and abstractions for embedded "
                   "peripherals and devices using modern C++")
    topics = ("peripherals", "hardware", "abstraction", "devices")
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True

    def package(self):
        self.copy("*LICENSE*", dst="licenses", keep_path=False)
        self.copy("*.h")
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
