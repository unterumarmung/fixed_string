from conans import ConanFile, CMake, tools


class FixedStringConan(ConanFile):
    name = "fixed_string"
    version = "1.0"
    license = "MIT License"
    author = "Daniil Dudkin"
    url = "https://github.com/unterumarmung/fixed_string"
    description = "C++ library that provides a basic_fixed_string template that combines std::array fixed-size semantic and std::string semantic together"
    topics = ("cpp17", "string", "constexpr")
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_tests": [True, False],"build_examples":[True,False]}
    default_options = {"build_tests": False,"build_examples":False}
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/unterumarmung/fixed_string.git")

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.definitions["FIXED_STRING_OPT_BUILD_EXAMPLES"] = self.options.build_examples
        cmake.definitions["FIXED_STRING_OPT_BUILD_TESTS"] = self.options.build_tests
        
        cmake.configure(source_folder="fixed_string")
        cmake.build()
        if self.options.build_tests:
            print("Running tests...")
            cmake.test()


    def package(self):
        self.copy("*.h", dst="include", src="fixed_string/include")
        self.copy("*.hpp", dst="include", src="fixed_string/include")
        
    def package_info(self):
        self.cpp_info.libs = ["fixed_string"]

