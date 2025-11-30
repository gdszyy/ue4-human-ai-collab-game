// Copyright Echo Alchemist Game. All Rights Reserved.

#include "Combat/CircularSceneManager.h"

UCircularSceneManager::UCircularSceneManager()
{
	Center = FVector::ZeroVector;
	InnerRadius = 300.0f;
	OuterRadius = 500.0f;
	EnemyRadius = 400.0f;
	BounceCoefficient = 0.8f;
}

void UCircularSceneManager::Initialize(float InInnerRadius, float InOuterRadius)
{
	InnerRadius = InInnerRadius;
	OuterRadius = InOuterRadius;
	EnemyRadius = (InnerRadius + OuterRadius) / 2.0f;
	
	UE_LOG(LogTemp, Log, TEXT("CircularSceneManager: Initialized with InnerRadius=%.1f, OuterRadius=%.1f"), 
		InnerRadius, OuterRadius);
}

void UCircularSceneManager::SetCenter(FVector InCenter)
{
	Center = InCenter;
	
	UE_LOG(LogTemp, Log, TEXT("CircularSceneManager: Center set to (%.1f, %.1f, %.1f)"), 
		Center.X, Center.Y, Center.Z);
}

void UCircularSceneManager::Advance(FVector& Position, FVector& Velocity, float DeltaTime)
{
	// 转换到相对坐标
	FVector RelativePosition = Position - Center;
	
	// 更新位置
	RelativePosition += Velocity * DeltaTime;
	
	// 转换回世界坐标
	Position = RelativePosition + Center;
}

FVector UCircularSceneManager::GetLaunchVector(const FVector& StartPosition, float TargetAngle)
{
	// 转换到相对坐标
	FVector RelativeStart = StartPosition - Center;
	
	// 计算当前角度和半径
	float CurrentRadius, CurrentAngle;
	CartesianToPolar(RelativeStart, CurrentRadius, CurrentAngle);
	
	// 归一化目标角度
	TargetAngle = NormalizeAngle(TargetAngle);
	
	// 计算目标位置（在敌人圆环上）
	FVector TargetRelative = PolarToCartesian(EnemyRadius, TargetAngle);
	
	// 计算发射向量（从当前位置指向目标位置）
	FVector LaunchVector = TargetRelative - RelativeStart;
	LaunchVector.Normalize();
	
	return LaunchVector;
}

bool UCircularSceneManager::IsOutOfBounds(const FVector& Position) const
{
	// 转换到相对坐标
	FVector RelativePosition = Position - Center;
	
	// 计算半径
	float Radius = RelativePosition.Size2D();
	
	// 检查是否超出外半径或小于内半径
	return (Radius > OuterRadius) || (Radius < InnerRadius);
}

void UCircularSceneManager::ApplyEnvironmentalForces(FVector& Velocity, float DeltaTime)
{
	// 环形场景没有重力，但可以添加其他环境力
	// 例如：向心力、离心力、阻力等
	
	// 这里暂时不添加环境力
}

void UCircularSceneManager::HandleBoundaryInteraction(FVector& Position, FVector& Velocity)
{
	// 转换到相对坐标
	FVector RelativePosition = Position - Center;
	
	// 计算半径和角度
	float Radius, Angle;
	CartesianToPolar(RelativePosition, Radius, Angle);
	
	// 检查是否超出外半径
	if (Radius > OuterRadius)
	{
		// 限制到外半径
		Radius = OuterRadius;
		
		// 计算法向量（径向向内）
		FVector Normal = -PolarToCartesian(1.0f, Angle);
		
		// 反弹速度
		Velocity = Velocity - 2.0f * FVector::DotProduct(Velocity, Normal) * Normal;
		Velocity *= BounceCoefficient;
		
		UE_LOG(LogTemp, Verbose, TEXT("CircularSceneManager: Boundary bounce (outer) at angle %.1f"), 
			FMath::RadiansToDegrees(Angle));
	}
	// 检查是否小于内半径
	else if (Radius < InnerRadius)
	{
		// 限制到内半径
		Radius = InnerRadius;
		
		// 计算法向量（径向向外）
		FVector Normal = PolarToCartesian(1.0f, Angle);
		
		// 反弹速度
		Velocity = Velocity - 2.0f * FVector::DotProduct(Velocity, Normal) * Normal;
		Velocity *= BounceCoefficient;
		
		UE_LOG(LogTemp, Verbose, TEXT("CircularSceneManager: Boundary bounce (inner) at angle %.1f"), 
			FMath::RadiansToDegrees(Angle));
	}
	
	// 更新位置
	RelativePosition = PolarToCartesian(Radius, Angle);
	Position = RelativePosition + Center;
}

FVector UCircularSceneManager::WorldToScreen(const FVector& LogicalPosition) const
{
	// 环形场景的逻辑坐标就是世界坐标
	return LogicalPosition;
}

FVector UCircularSceneManager::ScreenToWorld(const FVector& ScreenPosition) const
{
	// 环形场景的世界坐标就是逻辑坐标
	return ScreenPosition;
}

void UCircularSceneManager::CartesianToPolar(const FVector& CartesianPosition, float& OutRadius, float& OutAngle) const
{
	// 计算半径（2D距离）
	OutRadius = CartesianPosition.Size2D();
	
	// 计算角度（使用atan2）
	OutAngle = FMath::Atan2(CartesianPosition.Y, CartesianPosition.X);
	
	// 归一化角度到[0, 2π)
	OutAngle = NormalizeAngle(OutAngle);
}

FVector UCircularSceneManager::PolarToCartesian(float Radius, float Angle) const
{
	// 计算笛卡尔坐标
	float X = Radius * FMath::Cos(Angle);
	float Y = Radius * FMath::Sin(Angle);
	
	return FVector(X, Y, 0.0f);
}

FVector UCircularSceneManager::GetEnemyPosition(float Angle) const
{
	// 归一化角度
	Angle = NormalizeAngle(Angle);
	
	// 计算相对位置
	FVector RelativePosition = PolarToCartesian(EnemyRadius, Angle);
	
	// 转换到世界坐标
	return RelativePosition + Center;
}

float UCircularSceneManager::UpdateEnemyAngle(float CurrentAngle, float AngularVelocity, float DeltaTime) const
{
	// 更新角度
	float NewAngle = CurrentAngle + AngularVelocity * DeltaTime;
	
	// 归一化角度
	return NormalizeAngle(NewAngle);
}

float UCircularSceneManager::NormalizeAngle(float Angle)
{
	// 归一化角度到[0, 2π)
	const float TwoPI = 2.0f * PI;
	
	while (Angle < 0.0f)
	{
		Angle += TwoPI;
	}
	
	while (Angle >= TwoPI)
	{
		Angle -= TwoPI;
	}
	
	return Angle;
}

float UCircularSceneManager::AngleDifference(float Angle1, float Angle2)
{
	// 归一化角度
	Angle1 = NormalizeAngle(Angle1);
	Angle2 = NormalizeAngle(Angle2);
	
	// 计算角度差
	float Diff = Angle2 - Angle1;
	
	// 归一化到[-π, π]
	if (Diff > PI)
	{
		Diff -= 2.0f * PI;
	}
	else if (Diff < -PI)
	{
		Diff += 2.0f * PI;
	}
	
	return Diff;
}
