// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"

ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsWind = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Call Taichi Interface, add ball in scene
	for (auto el_BallMsg : this->BallMsgList)
	{
		//TODO
		// TaichiMoudel::AddSceneBall(el_BallMsg.Radius, el_BallMsg.Center);
	}

	InitClothingPosition();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("OnOffWindpower", IE_Pressed, this, &ASCharacter::OnOffWindpower);
}


void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}


void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

float ASCharacter::rand_P(float min, float max)
{
	return (min + (max - min) * (rand() / (float)RAND_MAX));
}


void ASCharacter::OnOffWindpower()
{
	if (this->IsWind == false)
	{
		this->IsWind = true;
		this->windPower.X = rand_P(1, 10);
		this->windPower.Y = rand_P(1, 10);
		this->windPower.Z = rand_P(1, 10);
	}
	else
	{
		this->IsWind = false;
		this->windPower = FVector(0,0,0);
	}

	//TODO
	//  call Taichi Interface, change wind power
	// TaichiMoudel::updateWindPower(this->windPower);
}


TArray<FVector> ASCharacter::computeVertices(FVector location, float radius, float halfHeight)
{
	float x = location.X;
	float y = location.Y;
	float z = location.Z;

	float bottom = z - halfHeight;
	float top = z + halfHeight;

	TArray<FVector> VerticesArray;

	VerticesArray.Add(FVector(x - radius, y - radius, bottom));
	VerticesArray.Add(FVector(x - radius, y + radius, bottom));
	VerticesArray.Add(FVector(x + radius, y - radius, bottom));
	VerticesArray.Add(FVector(x + radius, y + radius, bottom));


	VerticesArray.Add(FVector(x - radius, y - radius, top));
	VerticesArray.Add(FVector(x - radius, y + radius, top));
	VerticesArray.Add(FVector(x + radius, y - radius, top));
	VerticesArray.Add(FVector(x + radius, y + radius, top));
	return VerticesArray;
}

void ASCharacter::ShowHelpMsg()
{
	for (auto el_BallMsg : this->BallMsgList)
	{
		FString outputMessage1 = FString::Printf(TEXT("Scene Ball Msg: Center: %f, %f, %f, Raisu: %f."), el_BallMsg.Center.X, el_BallMsg.Center.Y, el_BallMsg.Center.Z, el_BallMsg.Radius);
		GEngine->AddOnScreenDebugMessage
		(
			-1,
			0, 			//	显示的时间/秒
			FColor::White, 	//	显示的颜色
			outputMessage1	//	显示的信息
		);
		// TaichiMoudel::AddSceneBall(el_BallMsg.Radius, el_BallMsg.Center);
	}

	FString outputMessage2;
	if (this->IsWind == false)
	{
		outputMessage2 = TEXT("windpower: off");
	}
	else
	{
		outputMessage2 = FString::Printf(TEXT("windpower: on, Value:(%f, %f, %f)"), this->windPower.X, this->windPower.Y, this->windPower.Z);
	}


	GEngine->AddOnScreenDebugMessage
	(
		-1,
		0, 			//	显示的时间/秒
		FColor::Blue, 	//	显示的颜色
		outputMessage2	//	显示的信息
	);
}

// TODO 调用Taichi接口， 初始化cloth顶点位置
void ASCharacter::InitClothingPosition()
{
	TArray<FVector> InitPosition;
	// TaichiMoudel::InitClothingPosition(InitPosition);

	return;
}

// TODO 更新cloth顶点位置 在TICK()中调用
void ASCharacter::UpdateClothingSkeletalMesh(TArray<FVector> NewPosition)
{



	return;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float half_height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector location = GetActorLocation();
	TArray<FVector> verticesPos = this->computeVertices(location, radius, half_height);

	DrawDebugBox(GetWorld(), location, FVector(radius, radius, half_height), FColor::Green);

	for (auto pos : verticesPos)
	{
		// UE_LOG(LogTemp, Log, TEXT("HealthChange:: %f, %f, %f"), pos.X, pos.Y, pos.Z);

		DrawDebugSphere(GetWorld(), pos, 3, 6, FColor::Red, false, 0);
	}

	// call Taichi Interface, 传入人物的碰撞盒信息
	// TaichiMoudel::updateCharacterCollisionBox(TArray<FVector>  verticesPos);

	TArray<FVector> NewPosition;
	// new pos return from Taichi Interface
	// NewPosition = TaichiMoudel::takeStep(int stepNum);


	this->UpdateClothingSkeletalMesh(NewPosition);


	this->ShowHelpMsg();
}