// Fill out your copyright notice in the Description page of Project Settings.


#include "ChallengeFunctionLibrary.h"

void UChallengeFunctionLibrary::BRC_Naive(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Naive"));
	// open file
	const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
	TUniquePtr<FArchive> File(IFileManager::Get().CreateFileReader(*Path));
	
	// Prepare RET
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Reading data"));
		
		int soonToBeEnum = 0;
		// 100 bytes, plus termination symbol
		constexpr int bufferSize = 101;
		ANSICHAR buffer[bufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		buffer[0] = 0;
		int bufferIndex = 0;
	
		FString Station;
		
		while(!File->AtEnd())
		{
			ANSICHAR byte;
		
			// this line stream from File to byte. Do not ask.
			*File << byte;
		
			if (soonToBeEnum == 0 && byte == ';')
			{
				buffer[bufferIndex] = 0;
				Station = UTF8_TO_TCHAR(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 1;
			}
			else if (soonToBeEnum == 1 && byte == '\n')
			{
				buffer[bufferIndex] = 0;
				double candidate = FCStringAnsi::Atof(buffer);
				buffer[0] = 0;
				bufferIndex = 0;
				soonToBeEnum = 0;
			
				//here add value to TMap
				if (FBRCStruct_Naive* row = Data.Find(Station))
				{
					row->min = row->min < candidate ? row->min : candidate;
					row->max = row->max > candidate ? row->max : candidate;
					row->avg = (row->avg * row->count + candidate) / (row->count + 1);
					row->count = row->count + 1; 
				}
				else
				{
					Data.Add(Station, FBRCStruct_Naive(candidate));
				}
			
			}
			else
			{
				buffer[bufferIndex] = byte;
				++bufferIndex;
			}
		}
	}
	
	// sort
	TArray<FString> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Sorting data"));
		Data.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Printing data"));
		for (auto key : SortedKeys)
		{
			FBRCStruct_Naive* row = Data.Find(key);
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row->min, row->avg, row->max);
		}
	}
	
	File->Close();
}