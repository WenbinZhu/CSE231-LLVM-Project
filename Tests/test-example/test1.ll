; ModuleID = 'test1.bc'
source_filename = "test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @foo(i32) local_unnamed_addr #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store volatile i32 %0, i32* %2, align 4
  %4 = bitcast i32* %3 to i8*
  call void @llvm.lifetime.start(i64 4, i8* %4)
  store volatile i32 10, i32* %3, align 4
  %5 = load volatile i32, i32* %2, align 4
  %6 = icmp ugt i32 %5, 5
  %7 = load volatile i32, i32* %3, align 4
  br i1 %6, label %8, label %10

; <label>:8:                                      ; preds = %1
  %9 = add i32 %7, 5
  store volatile i32 %9, i32* %3, align 4
  br label %12

; <label>:10:                                     ; preds = %1
  %11 = add i32 %7, 50
  store volatile i32 %11, i32* %3, align 4
  br label %12

; <label>:12:                                     ; preds = %10, %8
  %13 = load volatile i32, i32* %3, align 4
  %14 = load volatile i32, i32* %2, align 4
  %15 = add i32 %14, %13
  call void @llvm.lifetime.end(i64 4, i8* nonnull %4)
  ret i32 %15
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.1 (branches/release_39 291077)"}
