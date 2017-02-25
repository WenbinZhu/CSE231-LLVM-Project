; ModuleID = 'test.bc'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define i32 @f(i32) local_unnamed_addr #0 {
  %2 = icmp slt i32 %0, 14
  br i1 %2, label %3, label %5

; <label>:3:                                      ; preds = %1
  %4 = sdiv i32 5, %0
  br label %5

; <label>:5:                                      ; preds = %3, %1
  %6 = phi i32 [ 9, %3 ], [ 7, %1 ]
  %7 = phi i32 [ %4, %3 ], [ 7, %1 ]
  %8 = add nsw i32 %7, %6
  ret i32 %8
}

attributes #0 = { norecurse nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.1 (branches/release_39 291077)"}
