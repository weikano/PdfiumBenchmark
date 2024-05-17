cmake_minimum_required(VERSION 3.5)

project(PdfiumBenchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark REQUIRED)

set(PdfiumVersion 6476)
add_definitions(-DRESOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/testpdf\")
add_definitions(-DPDFIUM_VERSION=${PdfiumVersion})
add_definitions(-DPDFIUM_VERSION_STR=\"${PdfiumVersion}\")

include_directories(pdfium/${PdfiumVersion}/include)

add_executable(PdfiumBenchmark main.cpp benchmark_test.cpp pdfiumrenderer.cpp custommemorymanager.cpp save_sd_benchmark.cpp pdfwriter.cpp)
target_link_libraries(PdfiumBenchmark ${CMAKE_CURRENT_SOURCE_DIR}/pdfium/${PdfiumVersion}/lib/libpdfium.so)
target_link_libraries(PdfiumBenchmark benchmark::benchmark)
target_link_libraries(PdfiumBenchmark pthread m)

install(TARGETS PdfiumBenchmark
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
