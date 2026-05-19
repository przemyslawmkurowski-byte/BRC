// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChallengeFunctionLibrary.generated.h"

struct FBRCStruct_Naive
{
	FBRCStruct_Naive() : min(-100), max(100), avg(0), count(0) {}
	FBRCStruct_Naive(double value) : min(value), max(value), avg(value), count(1) {}
	
	// specification limit range to <-99.9, 99.9>
	double min;
	double max;
	double avg;
	double count;
};

struct FByteKey
{
	ANSICHAR Buffer[101];	// 100 bytes + termination marker
	
    bool operator==(const FByteKey& Other) const
    {
    	for(int i = 0; Buffer[i] && Other.Buffer[i]; i++)
    	{
    		if(Buffer[i] != Other.Buffer[i])
    			return false;
    	}
        return true;
    }
	
	bool operator<(const FByteKey& Other) const
    {
	    int i = 0;
    	for ( ; Buffer[i] != Other.Buffer[i] && i < 100; ++i)
    		;	// get to first i at which Buffers differ
    	return Buffer[i] < Other.Buffer[i];
    }
	
    friend uint32 GetTypeHash(const FByteKey& Key)
    {
        uint32 Hash = 0;
        for(int i = 0; Key.Buffer[i]; ++i)
        	Hash = Hash * 31 + Key.Buffer[i]; 
    	
        return Hash;
    }
	
	FString ToString() const
    {
    	return UTF8_TO_TCHAR(Buffer);
    }
};


/**
 * 
 */
UCLASS()
class BRC_API UChallengeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void BRC_Naive(const FString& InInputName);
	
	// Use FName instead of FString, as key for TMap
	UFUNCTION(BlueprintCallable)
	static void BRC_Name(const FString& InInputName);
	
	// Load 4096 bytes at once. Should give CPU more time between calls to hard disc.
    UFUNCTION(BlueprintCallable)
    static void BRC_Chunk(const FString& InInputName);
    
    // Uses char[] instead of FString as TMap key
    UFUNCTION(BlueprintCallable)
    static void BRC_Hash(const FString& InInputName);
	
	// Using Load File To String Array. Should massively increase RAM usage, checking how it will affect CPU usage
	UFUNCTION(BlueprintCallable)
    static void BRC_LFTSA(const FString& InInputName);
	
	// separate TMAP<Station, index> and array< FBRCStruct_Naive
	UFUNCTION(BlueprintCallable)
	static void BRC_TSI(const FString& InInputName);
	
	// prereserved array size
	UFUNCTION(BlueprintCallable)
    static void BRC_TSI_PAS(const FString& InInputName);
};
