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

#include <jni.h>
#include <windows.h>

static int checkStackLimit() {
  PNT_TIB old_tib = reinterpret_cast<PNT_TIB>(NtCurrentTeb());
  ULONG_PTR before = reinterpret_cast<ULONG_PTR>(old_tib->StackLimit);

  const size_t offset = 256 * 1024;
  volatile unsigned char* fault_addr =
      reinterpret_cast<volatile unsigned char*>(before - offset);

  // Fault below StackLimit to exercise HotSpot's stack commitment recovery.
  *fault_addr = 0;

  PNT_TIB new_tib = reinterpret_cast<PNT_TIB>(NtCurrentTeb());
  ULONG_PTR after = reinterpret_cast<ULONG_PTR>(new_tib->StackLimit);
  ULONG_PTR faulted = reinterpret_cast<ULONG_PTR>(fault_addr);
  return after <= faulted ? 0 : 1;
}

extern "C" JNIEXPORT jint JNICALL
Java_TestWindowsStackLimit_checkStackLimit(JNIEnv*, jclass) {
  // Resolve the library without attempting the fault in _thread_in_native.
  return -1;
}

extern "C" JNIEXPORT jint JNICALL
JavaCritical_TestWindowsStackLimit_checkStackLimit() {
  return checkStackLimit();
}
