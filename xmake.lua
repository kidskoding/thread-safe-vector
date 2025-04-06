set_languages("c++17")
add_requires("gtest")

target("thread-safe-vector")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("src")

target("tests")
    set_kind("static")
    add_files("src/*.cpp")
    add_tests("tests/*.cpp")
    add_packages("gtest")
    add_includedirs("src")

add_rules("mode.debug", "mode.release")
