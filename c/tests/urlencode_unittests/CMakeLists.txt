#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for urlencode_unittests
cmake_minimum_required(VERSION 2.8.11)

compileAsC11()
set(theseTestsName urlencode_unittests)

set(${theseTestsName}_cpp_files
${theseTestsName}.cpp
)

set(${theseTestsName}_c_files
../../src/urlencode.c

../../src/strings.c
../../src/gballoc.c
${LOCK_C_FILE}
)

set(${theseTestsName}_h_files
)

build_test_artifacts(${theseTestsName} ON)