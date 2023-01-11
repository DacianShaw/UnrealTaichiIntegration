// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Animation/SkeletalMeshActor.h"
#include "ChaosCloth/ChaosClothingSimulationFactory.h"
#include "ChaosCloth/ChaosClothingSimulation.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	IsWind = false;
	NumSize = 32;
	NumParticles = NumSize * NumSize;
	QuadSize = ClothSize / NumSize;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ����Taichi�ӿ�, ��ӳ����еľ�̬����
	for (auto el_BallMsg : this->BallMsgList)
	{
		// TaichiMoudel::AddSceneBall(el_BallMsg.Radius, el_BallMsg.Center);
	}
	QuadSize = ClothSize / NumSize;

	InitClothingPosition();
}

float ASCharacter::rand_P(float min, float max)
{
	return (min + (max - min) * (rand() / (float)RAND_MAX));
}

// ���Ʒ���
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
		this->windPower = FVector(0, 0, 0);
	}

	//  ����Taihci�ӿڣ� ���Ʒ���
	// TaichiMoudel::updateWindPower(this->windPower);
}

// �����ɫ��ײ���嶥��λ��
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

// ������ʾ
void ASCharacter::ShowHelpMsg()
{
	for (auto el_BallMsg : this->BallMsgList)
	{
		FString outputMessage1 = FString::Printf(TEXT("Scene Ball Msg: Center: %f, %f, %f, Raisu: %f."), el_BallMsg.Center.X, el_BallMsg.Center.Y, el_BallMsg.Center.Z, el_BallMsg.Radius);
		GEngine->AddOnScreenDebugMessage
		(
			-1,
			0, 			//	��ʾ��ʱ��/��
			FColor::White, 	//	��ʾ����ɫ
			outputMessage1	//	��ʾ����Ϣ
		);
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
		0, 			//	��ʾ��ʱ��/��
		FColor::Blue, 	//	��ʾ����ɫ
		outputMessage2	//	��ʾ����Ϣ
	);
}

// TODO ����Taichi�ӿڣ� ��ʼ��cloth����λ��
void ASCharacter::InitClothingPosition()
{
	int8 RowIndex = 0;
	int8 ColIndex = 0;
	int32 CountNum = 0;

	TArray<FVector3f> positionArray;
	positionArray.Init(FVector3f::ZeroVector, NumParticles);

	//  ��ʼ��cloth����λ��
	for (auto& pos : positionArray)
	{
		RowIndex = CountNum / NumSize;
		ColIndex = CountNum % NumSize;
		pos = FVector3f(RowIndex * QuadSize, ColIndex * QuadSize, 0);
		CountNum += 1;
	}

	TArray<FVector3f> NormalsArray;
	NormalsArray.Init(FVector3f(0.0, 0.0, -1.0), NumParticles);

	TaichiClothSimulData = new FClothSimulData();
	TaichiClothSimulData->LODIndex = 0;
	TaichiClothSimulData->Transform = FTransform(FQuat4d::Identity, FVector3d(this->ClothLocation), FVector3d(1, 1, 1));
	TaichiClothSimulData->Normals = NormalsArray; 
	TaichiClothSimulData->Positions = positionArray;

	FActorSpawnParameters Params;
	Params.Owner = this;
	ClothActor = Cast<ASkeletalMeshActor>(GetWorld()->SpawnActor<AActor>(ClothActorClass, ClothLocation, FRotator::ZeroRotator, Params));
	ClothMeshComponent = ClothActor->GetSkeletalMeshComponent();
	ClothMeshComponent->IsTaichiSimulation = true;
	TaichiClothSimulData->ComponentRelativeTransform = FTransform(FQuat4d::Identity, FVector3d::ZeroVector, FVector3d(1, 1, 1) / ClothActor->GetActorScale3D());

	TMap<int32, FClothSimulData>* TaichiSimulationData =  new TMap<int32, FClothSimulData>;
	TaichiSimulationData->Add(0, *TaichiClothSimulData);
	ClothMeshComponent->CurrentSimulationData_Taichi = *TaichiSimulationData;


	{
		TArray<FVector3f> InitPosition;
		InitPosition.Init(FVector3f::ZeroVector, NumParticles);

		for (int i =0;i<NumParticles;i++)
		{
			InitPosition[i] = positionArray[i] + FVector3f(ClothLocation);
		}

		// ����Taihci�ӿڣ���ʼ������λ�ã� ���붥����������꣬ ��λcm
		// TaichiMoudel::InitClothingPosition(InitPosition);
	}
}

// TODO ����cloth����λ�� ��TICK()�е���
void ASCharacter::UpdateClothingSkeletalMesh(TArray<FVector3f> NewPosition)
{
	
	for (int i = 0; i < 1024; i++)
	{
		NewPosition[i] -= FVector3f(ClothLocation);   //ת��Ϊ�������
	}
	TaichiClothSimulData->Positions = NewPosition;
	ClothMeshComponent->CurrentSimulationData_Taichi.Add(0, *TaichiClothSimulData);
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
		DrawDebugSphere(GetWorld(), pos, 3, 6, FColor::Red, false, 0);
	}

	// ����Taichi�ӿ�, �����������ײ����Ϣ
	// TaichiMoudel::updateCharacterCollisionBox(TArray<FVector>  verticesPos);

	TArray<FVector3f> NewPosition;

	{ //ʵ��taichi�ӿں�ע�ʹ˲������� 208 - 231
		NewPosition = TaichiClothSimulData->Positions;
		if (NewPosition.IsEmpty())
		{
			int8 RowIndex = 0;
			int8 ColIndex = 0;
			int32 CountNum = 0;

			TArray<FVector3f> positionArray;
			positionArray.Init(FVector3f::ZeroVector, NumParticles);

			for (auto& pos : positionArray)
			{
				RowIndex = CountNum / NumSize;
				ColIndex = CountNum % NumSize;
				pos = FVector3f(RowIndex * QuadSize, ColIndex * QuadSize, 0);
				CountNum += 1;
			}

			NewPosition = positionArray;
		}
		for (auto& pos : NewPosition) { pos += FVector3f(ClothLocation); }
		for (auto& pos : NewPosition) { pos.Z -= 1.0/5.0; }
	}

	// ����Taichi�ӿ�,��ȡ���º��λ��
	// NewPosition = TaichiMoudel::takeStep(int stepNum);

	this->UpdateClothingSkeletalMesh(NewPosition);

	this->ShowHelpMsg();
}


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