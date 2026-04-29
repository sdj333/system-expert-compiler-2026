; RUN: opt -passes=sccp -S %s | FileCheck %s

define i32 @main() {
  br i1 false, label %A, label %B
A:
  ret i32 1
B:
  ret i32 2
}

; CHECK-LABEL: define i32 @main()
; CHECK: br label %B
; CHECK: ret i32 2
; CHECK-NOT: A:
; CHECK-NOT: ret i32 1
