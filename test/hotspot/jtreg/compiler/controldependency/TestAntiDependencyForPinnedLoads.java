/*
 * Copyright (c) 2024, Oracle and/or its affiliates. All rights reserved.
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

/**
 * @test
 * @bug 8337066
 * @summary Test that MergeMem is skipped when looking for stores
 * @compile -encoding UTF-8 TestAntiDependencyForPinnedLoads.java
 * @run main/othervm -Xbatch -XX:-TieredCompilation
 *                   -XX:CompileCommand=compileonly,java.lang.StringUTF16::reverse
 *                   compiler.controldependency.TestAntiDependencyForPinnedLoads
 */

package compiler.controldependency;

public class TestAntiDependencyForPinnedLoads {
    public static void main(String[] args) {
        for(int i = 0; i < 50_000; i++) {
            String str = "YYYY年MM月DD日";
            StringBuffer strBuffer = new StringBuffer(str);
            String revStr = strBuffer.reverse().toString();
            if (!revStr.equals("日DD月MM年YYYY")) throw new InternalError("FAIL");
        }
    }
}
