from conans import ConanFile, CMake

class AyConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "glfw/3.3@bincrafters/stable", "stb/20190512@conan/stable", "glad/0.1.29@bincrafters/stable", "glm/0.9.9.5@g-truc/stable"
    generators = "cmake"
    default_options = {}

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()