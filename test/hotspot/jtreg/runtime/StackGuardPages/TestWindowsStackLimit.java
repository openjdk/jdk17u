/*
 * Copyright (c) 2026, Microsoft and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

/*
 * @test
 * @summary Verifies that on Windows, HotSpot's access-violation recovery for
 *          an in-Java stack fault advances TEB.StackLimit to cover the newly
 *          committed stack memory.
 * @requires os.family == "windows" & os.arch == "amd64"
 * @run main/othervm/native -Xcomp -XX:+CriticalJNINatives
 *      -XX:CompileCommand=compileonly,TestWindowsStackLimit::*
 *      TestWindowsStackLimit
 */

public class TestWindowsStackLimit {
    // Critical JNI keeps the thread in _thread_in_Java, as required by
    // topLevelExceptionFilter() when committing stack memory.
    private static native int checkStackLimit();

    public static void main(String[] args) {
        System.loadLibrary("StackLimit");

        int result = checkStackLimit();
        if (result == -1) {
            throw new RuntimeException("Critical JNI entry was not used");
        }
        if (result != 0) {
            throw new RuntimeException("TEB.StackLimit was not advanced");
        }
    }
}
