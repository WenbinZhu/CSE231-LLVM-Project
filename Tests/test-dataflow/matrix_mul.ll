; ModuleID = 'matrix_mul.bc'
source_filename = "matrix_mul.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @_Z10matrix_mulv() local_unnamed_addr #0 {
  %1 = alloca [10 x [10 x i32]], align 16
  %2 = alloca [10 x [10 x i32]], align 16
  %3 = alloca [10 x [10 x i32]], align 16
  %4 = bitcast [10 x [10 x i32]]* %1 to i8*
  call void @llvm.lifetime.start(i64 400, i8* %4) #3
  %5 = bitcast [10 x [10 x i32]]* %2 to i8*
  call void @llvm.lifetime.start(i64 400, i8* %5) #3
  %6 = bitcast [10 x [10 x i32]]* %3 to i8*
  call void @llvm.lifetime.start(i64 400, i8* %6) #3
  tail call void @srand(i32 7) #3
  br label %7

; <label>:7:                                      ; preds = %9, %0
  %8 = phi i64 [ 0, %0 ], [ %10, %9 ]
  br label %12

; <label>:9:                                      ; preds = %12
  %10 = add nuw nsw i64 %8, 1
  %11 = icmp eq i64 %10, 10
  br i1 %11, label %23, label %7

; <label>:12:                                     ; preds = %12, %7
  %13 = phi i64 [ 0, %7 ], [ %21, %12 ]
  %14 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %3, i64 0, i64 %8, i64 %13
  store i32 0, i32* %14, align 4, !tbaa !1
  %15 = tail call i32 @rand() #3
  %16 = srem i32 %15, 100
  %17 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %1, i64 0, i64 %8, i64 %13
  store i32 %16, i32* %17, align 4, !tbaa !1
  %18 = tail call i32 @rand() #3
  %19 = srem i32 %18, 100
  %20 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %2, i64 0, i64 %8, i64 %13
  store i32 %19, i32* %20, align 4, !tbaa !1
  %21 = add nuw nsw i64 %13, 1
  %22 = icmp eq i64 %21, 10
  br i1 %22, label %9, label %12

; <label>:23:                                     ; preds = %9, %30
  %24 = phi i64 [ %31, %30 ], [ 0, %9 ]
  br label %26

; <label>:25:                                     ; preds = %30
  call void @llvm.lifetime.end(i64 400, i8* nonnull %6) #3
  call void @llvm.lifetime.end(i64 400, i8* nonnull %5) #3
  call void @llvm.lifetime.end(i64 400, i8* nonnull %4) #3
  ret void

; <label>:26:                                     ; preds = %33, %23
  %27 = phi i64 [ 0, %23 ], [ %34, %33 ]
  %28 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %3, i64 0, i64 %24, i64 %27
  %29 = load i32, i32* %28, align 4, !tbaa !1
  br label %36

; <label>:30:                                     ; preds = %33
  %31 = add nuw nsw i64 %24, 1
  %32 = icmp eq i64 %31, 10
  br i1 %32, label %25, label %23

; <label>:33:                                     ; preds = %36
  store i32 %44, i32* %28, align 4, !tbaa !1
  %34 = add nuw nsw i64 %27, 1
  %35 = icmp eq i64 %34, 10
  br i1 %35, label %30, label %26

; <label>:36:                                     ; preds = %36, %26
  %37 = phi i64 [ 0, %26 ], [ %45, %36 ]
  %38 = phi i32 [ %29, %26 ], [ %44, %36 ]
  %39 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %1, i64 0, i64 %24, i64 %37
  %40 = load i32, i32* %39, align 4, !tbaa !1
  %41 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %2, i64 0, i64 %37, i64 %27
  %42 = load i32, i32* %41, align 4, !tbaa !1
  %43 = mul nsw i32 %42, %40
  %44 = add nsw i32 %38, %43
  %45 = add nuw nsw i64 %37, 1
  %46 = icmp eq i64 %45, 10
  br i1 %46, label %33, label %36
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: nounwind
declare void @srand(i32) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @rand() local_unnamed_addr #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.1 (branches/release_39 291077)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C++ TBAA"}
