from conans import ConanFile, CMake, tools

import re

# extracts the project name and version from CMake configuration file
def get_infos():
    try:
        content = tools.load("CMakeLists.txt")
        infos = re.search(r"project\s*\(\s*(\w+)\s*\bVERSION\s*(\d+(\.\d+)*)", content, re.DOTALL)
        name = infos.group(1).strip()
        version = infos.group(2).strip()
        return name, version
    except Exception:
        return None,None

class cfllpspConan(ConanFile):
    name, version = get_infos()
    license = ""
    url = "https://github.com/SimonPiCarter/GameEngine"
    description = ""
    settings = "os", "compiler", "build_type", "arch"
    requires = "gtest/1.10.0"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    generators = "cmake", "cmake_paths"
    #build_policy = # can be never, missing, always
    #exports = # exports required files for setup
    #exports_sources = # exports required files for rebuilding (conan build)
    no_copy_source = True
    short_paths = True

    # manages options propagation
    # def config_options(self):

    # CMake configuration wrapper
    # def configure_cmake(self):

    # configure
    #def configure(self):

    # manages requirements
    # allows for defining optional requirements
    #def requirements(self):

    # copy required files from local store to project
    # handy for copying required shared libraries when testing
    #def imports(self):
    #    self.copy("*.dll", dst="bin", src="bin")
    #    self.copy("*.dylib*", dst="bin", src="lib")
    #    self.copy('*.so*', dst='bin', src='lib')

    # retrieves sources
    # def source(self):
    #     git = tools.Git()
    #     git.clone("git@edgitlab.eurodecision.com:cfl/cfllpsp.git", branch=self.version, shallow=True)

    # CMake configuration wrapper
    # def configure_cmake(self):
    #     cmake = CMake(self)
    #     if self.settings.os == "Windows":
    #         cmake.definitions["STATIC_RUNTIME"] = "OFF" if "md" in str(self.settings.compiler.runtime).lower() else "ON"
    #     cmake.configure()
    #     return cmake

    # builds project using standard CMake commands
    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    # copies artifacts
    # uses CMake installed files
    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    # defines required information for consuming package
    #def package_info(self):

