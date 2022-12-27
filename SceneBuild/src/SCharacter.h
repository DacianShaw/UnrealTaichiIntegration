// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


USTRUCT(BlueprintType)
struct FBallMsg {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float Radius;
	UPROPERTY(EditAnywhere)
		FVector Center;
};

UCLASS()
class SCENEDEMO_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "shot")
	TSubclassOf<AActor> StaticBallClass;

	float capsule_half_height;
	float capsule_radius;

	bool IsWind;
	FVector windPower;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnOffWindpower();
	TArray<FVector> computeVertices(FVector location, float radius, float halfHeight);
	float rand_P(float min, float max);
	void ShowHelpMsg();
	void InitClothingPosition();
	void UpdateClothingSkeletalMesh(TArray<FVector> NewPosition);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category="SceneBallMsg")
	TArray<FBallMsg> BallMsgList;
};
