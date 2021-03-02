// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VPmame.generated.h"

/**
 * 
 */
UCLASS()
class VPINBALL_API UVPmame : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;


	UFUNCTION(BlueprintCallable, Category = "VPinball;VPinmame")
		static void VPStart(const FString& RomName);
	UFUNCTION(BlueprintCallable, Category = "VPinball;VPinmame")
		static void VPStop();
	UFUNCTION(BlueprintCallable, Category = "VPinball;VPinmame")
		static void VPGetDMD(TArray<uint8>& Dots);
	UFUNCTION(BlueprintCallable, Category = "VPinball;VPinmame")
		static void VPGetLamps(TArray<uint8>& Lamps);
};
