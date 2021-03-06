#
# Copyright 2017, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#
#     * Neither the name of the copyright holder nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

set(TEST_APP "./test_syscalls")

set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

add_executable(test_syscalls test_syscalls.c)

#
# TESTS
#

add_test(NAME "basic-syscalls"
	COMMAND ${TEST_DIR}/test-match ${TEST_APP} 0)

add_test(NAME "basic-syscalls-with-fork"
	COMMAND ${TEST_DIR}/test-match -f ${TEST_APP} 1)

add_test(NAME "basic-syscalls-with-double-fork"
	COMMAND ${TEST_DIR}/test-match -f ${TEST_APP} 7)

add_test(NAME "unsupported-syscalls"
	COMMAND ${TEST_DIR}/test-match ${TEST_APP} 2)

add_test(NAME "unsupported-syscalls-with-fork"
	COMMAND ${TEST_DIR}/test-match -f ${TEST_APP} 3)

add_test(NAME "unsupported-syscalls-with-double-fork"
	COMMAND ${TEST_DIR}/test-match -f ${TEST_APP} 8)

add_test(NAME "syscall-vfork"
	COMMAND ${TEST_DIR}/test-match ${TEST_APP} 4)

add_test(NAME "syscall-umount"
	COMMAND ${TEST_DIR}/test-match ${TEST_APP} 5)

add_test(NAME "syscall-xattr"
	COMMAND ${TEST_DIR}/test-match ${TEST_APP} 6)
