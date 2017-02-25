; ModuleID = 'fib.bc'
source_filename = "fib.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define i32 @calc_fib(i32) local_unnamed_addr #0 {
  %2 = icmp ult i32 %0, 2
  %3 = icmp slt i32 %0, 2
  %4 = or i1 %2, %3
  br i1 %4, label %12, label %5

; <label>:5:                                      ; preds = %1, %5
  %6 = phi i32 [ %10, %5 ], [ 2, %1 ]
  %7 = phi i32 [ %8, %5 ], [ 1, %1 ]
  %8 = phi i32 [ %9, %5 ], [ 1, %1 ]
  %9 = add nsw i32 %7, %8
  %10 = add nuw nsw i32 %6, 1
  %11 = icmp eq i32 %6, %0
  br i1 %11, label %12, label %5

; <label>:12:                                     ; preds = %5, %1
  %13 = phi i32 [ 1, %1 ], [ %9, %5 ]
  ret i32 %13
}

attributes #0 = { norecurse nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.1 (branches/release_39 291077)"}
