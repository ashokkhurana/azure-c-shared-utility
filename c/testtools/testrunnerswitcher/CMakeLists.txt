#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists for testrunerswitcher. It only defines a global include 
cmake_minimum_required(VERSION 2.8.11)

compileAsC99()

#these are the include folders
#the following "set" statetement exports across the project a global variable called TESTRUNNERSWITCHER_INC_FOLDER that expands to whatever needs to included when using tesrtrunnerswitcher library
set(TESTRUNNERSWITCHER_INC_FOLDER ${CMAKE_CURRENT_LIST_DIR}/inc CACHE INTERNAL "this is what needs to be included if using testrunnerswitcher header" FORCE)

add_files_to_install("./testtools/testrunnerswitcher/inc/testrunnerswitcher.h")
