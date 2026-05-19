// Fill out your copyright notice in the Description page of Project Settings.

#include "ChallengeFunctionLibrary.h"

void UChallengeFunctionLibrary::BRC_Naive(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Naive"));
	// open file
	FArchive* File;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Open file"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		File = IFileManager::Get().CreateFileReader(*Path);
	}
	
	// Prepare RET
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Read File >> Data"));
		
		// 100 bytes, plus termination symbol
		constexpr int BufferSize = 101;
		ANSICHAR Buffer[BufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		Buffer[0] = 0;
		int BufferIndex = 0;
		ANSICHAR Expected = ';';	// double-use as Stage flag
		
		FString Station;	// need to keep this one through Loop iterations

		while(!File->AtEnd())
		{
			ANSICHAR Byte;
			// this line stream from File to byte. Do not ask.
			*File << Byte;
			
			if(Byte != Expected)	// EARLY EXIT
			{
				Buffer[BufferIndex] = Byte;
				++BufferIndex;
				continue;
			}
			
			//at this point we are sure Byte == Expected
			if (Expected == ';')
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				Station = UTF8_TO_TCHAR(Buffer);
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = '\n';
			}
			else // there are only two options - either expected is ; or \n
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				double Value = FCStringAnsi::Atof(Buffer);
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = ';';
					
				//here add value to TMap
				FBRCStruct_Naive* row = Data.Find(Station);
				if (row != nullptr)
				{
					row->min = row->min < Value ? row->min : Value;
					row->max = row->max > Value ? row->max : Value;
					row->avg = (row->avg * row->count + Value) / (row->count + 1);
					row->count = row->count + 1; 
				}
				else
				{
					Data.Add(Station, FBRCStruct_Naive(Value));
				}
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
	delete File;
}

void UChallengeFunctionLibrary::BRC_Name(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Name"));
	// open file
	FArchive* File;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Open file"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		File = IFileManager::Get().CreateFileReader(*Path);
	}
	
	// Prepare RET
	TMap<FName, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Read File >> Data"));
		
		// 100 bytes, plus termination symbol
		constexpr int BufferSize = 101;
		ANSICHAR Buffer[BufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		Buffer[0] = 0;
		int BufferIndex = 0;
		ANSICHAR Expected = ';';	// double-use as Stage flag
		
		FName Station;	// need to keep this one through Loop iterations

		while(!File->AtEnd())
		{
			ANSICHAR Byte;
			// this line stream from File to byte. Do not ask.
			*File << Byte;
			
			if(Byte != Expected)	// EARLY EXIT
			{
				Buffer[BufferIndex] = Byte;
				++BufferIndex;
				continue;
			}
			
			//at this point we are sure Byte == Expected
			if (Expected == ';')
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				Station = UTF8_TO_TCHAR(Buffer);
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = '\n';
			}
			else // there are only two options - either expected is ; or \n
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				double Value = FCStringAnsi::Atof(Buffer);
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = ';';
					
				//here add value to TMap
				FBRCStruct_Naive* row = row = Data.Find(Station);
				
				if (row != nullptr)
				{
					row->min = row->min < Value ? row->min : Value;
					row->max = row->max > Value ? row->max : Value;
					row->avg = (row->avg * row->count + Value) / (row->count + 1);
					row->count = row->count + 1; 
				}
				else
				{
					Data.Add(Station, FBRCStruct_Naive(Value));
				}
			}
		}
	}	
			
	// sort
	TArray<FName> SortedKeys;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Name::Sorting data"));
		Data.GetKeys(SortedKeys);
		SortedKeys.Sort(
			[](const FName& A, const FName& B)
			{
				return A.ToString() < B.ToString();
			});
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Printing data"));
		for (auto key : SortedKeys)
		{
			FBRCStruct_Naive* row = Data.Find(key);
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key.ToString(), row->min, row->avg, row->max);
		}
	}
	
	File->Close();
	delete File;
}

void UChallengeFunctionLibrary::BRC_Chunk(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Chunk"));
	// open file
	FArchive* File;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Open file"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		File = IFileManager::Get().CreateFileReader(*Path);
	}
	
	// Prepare RET
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Chunk::Reading data"));
		
		// 100 bytes, plus termination symbol
		constexpr int BufferSize = 101;
		ANSICHAR Buffer[BufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		Buffer[0] = 0;
		int BufferIndex = 0;
		ANSICHAR Expected = ';';	// double-use as Stage flag
	
		FString Station;
		
		constexpr int ChunkMaxSize = 4096;
		ANSICHAR Chunk[ChunkMaxSize];
		int ChunkActualSize = 0;
		
		while(!File->AtEnd())
		{
			const int BytesToEoF = File->TotalSize() - File->Tell();
			ChunkActualSize = BytesToEoF < ChunkMaxSize ? BytesToEoF : ChunkMaxSize;
			File->Serialize(Chunk, ChunkActualSize);
			
			for (int i = 0; i < ChunkActualSize; ++i)
			{
				ANSICHAR Byte = Chunk[i];
			
				if(Byte != Expected)	// EARLY EXIT
				{
					Buffer[BufferIndex] = Byte;
					++BufferIndex;
					continue;
				}
			
				//at this point we are sure Byte == Expected
				if (Expected == ';')
				{
					Buffer[BufferIndex] = 0; // Add termination marker to buffer
					Station = UTF8_TO_TCHAR(Buffer);
					Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
					BufferIndex = 0;
					Expected = '\n';
				}
				else // there are only two options - either expected is ; or \n
				{
					Buffer[BufferIndex] = 0; // Add termination marker to buffer
					double Value = FCStringAnsi::Atof(Buffer);
					Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
					BufferIndex = 0;
					Expected = ';';
					
					//here add value to TMap
					FBRCStruct_Naive* row = row = Data.Find(Station);
				
					if (row != nullptr)
					{
						row->min = row->min < Value ? row->min : Value;
						row->max = row->max > Value ? row->max : Value;
						row->avg = (row->avg * row->count + Value) / (row->count + 1);
						row->count = row->count + 1; 
					}
					else
					{
						Data.Add(Station, FBRCStruct_Naive(Value));
					}
				}
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
	delete File;
}

void UChallengeFunctionLibrary::BRC_Hash(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_Hash"));
	// open file
	FArchive* File;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Open file"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		File = IFileManager::Get().CreateFileReader(*Path);
	}
	
	// Prepare RET
	TMap<FByteKey, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Read File >> Data"));
		
		// 100 bytes, plus termination symbol
		constexpr int BufferSize = 101;
		ANSICHAR Buffer[BufferSize];
		// buffer is null-terminated, so making last character null should work just fine
		Buffer[0] = 0;
		int BufferIndex = 0;
		ANSICHAR Expected = ';';	// double-use as Stage flag
		
		FByteKey Station;	// need to keep this one through Loop iterations

		while(!File->AtEnd())
		{
			ANSICHAR Byte;
			// this line stream from File to byte. Do not ask.
			*File << Byte;
			
			if(Byte != Expected)	// EARLY EXIT
			{
				TRACE_CPUPROFILER_EVENT_SCOPE_CONDITIONAL(TEXT("BRC_Hash::Detailed log check"), false);
				Buffer[BufferIndex] = Byte;
				++BufferIndex;
				continue;
			}
			
			//at this point we are sure Byte == Expected
			if (Expected == ';')
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				for (int i = 0; Buffer[i]; ++i)
					Station.Buffer[i] = Buffer[i];
				Station.Buffer[BufferIndex] = 0;
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = '\n';
			}
			else // there are only two options - either expected is ; or \n
			{
				Buffer[BufferIndex] = 0; // Add termination marker to buffer
				double Value = FCStringAnsi::Atof(Buffer);
				Buffer[0] = 0; // just to be sure - clear buffer by putting termination at first index
				BufferIndex = 0;
				Expected = ';';
					
				//here add value to TMap
				FBRCStruct_Naive* row = Data.Find(Station);
				if (row != nullptr)
				{
					row->min = row->min < Value ? row->min : Value;
					row->max = row->max > Value ? row->max : Value;
					row->avg = (row->avg * row->count + Value) / (row->count + 1);
					row->count = row->count + 1; 
				}
				else
				{
					Data.Add(Station, FBRCStruct_Naive(Value));
				}
			}
		}
	}	
			
	// sort
	TArray<FByteKey> SortedKeys;
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
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key.ToString(), row->min, row->avg, row->max);
		}
	}
	
	File->Close();
	delete File;
}

void UChallengeFunctionLibrary::BRC_LFTSA(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_LFTSA"));
	
	// Load file to RAM
	TArray<FString> Lines;
	TMap<FString, FBRCStruct_Naive> Data;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_LFTSA::Loading data to RAM"));
		const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
		FFileHelper::LoadFileToStringArray(OUT Lines, *Path);
	}
	
	FString Separator = TEXT(";");
	FString Station, NumberAsString;
	
	for (int i = 0; i < Lines.Num(); ++i)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Reading data"));
		Lines[i].Split(Separator, &Station, &NumberAsString);
		double candidate = FCString::Atod(*NumberAsString);
		
		//here add value to TMap
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Find row"));
			if (FBRCStruct_Naive* row = Data.Find(Station))
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Recalc old value"));
				row->min = row->min < candidate ? row->min : candidate;
				row->max = row->max > candidate ? row->max : candidate;
				row->avg = (row->avg * row->count + candidate) / (row->count + 1);
				row->count = row->count + 1; 
			}
			else
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_Naive::Add new value"));
				Data.Add(Station, FBRCStruct_Naive(candidate));
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
}

void UChallengeFunctionLibrary::BRC_TSI(const FString& InInputName)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_TSI"));
	// open file
	const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
	TUniquePtr<FArchive> File(IFileManager::Get().CreateFileReader(*Path));
	
	// Prepare RET
	TMap<FString, int> Indices;
	TArray<FBRCStruct_Naive> Rows;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Reading data"));
		
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
			
				if(int* index = Indices.Find(Station))
				{
					FBRCStruct_Naive& row = Rows[*index];
					row.min = row.min < candidate ? row.min : candidate;
					row.max = row.max > candidate ? row.max : candidate;
					row.avg = (row.avg * row.count + candidate) / (row.count + 1);
				}
				else
				{
					const int currIndex = Rows.Num();
					Rows.Add(FBRCStruct_Naive(candidate));
					Indices.Add(Station, currIndex);
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
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Sorting data"));
		Indices.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Printing data"));
		for (auto key : SortedKeys)
		{
			int* index = Indices.Find(key);
			FBRCStruct_Naive& row = Rows[*index];
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row.min, row.avg, row.max);
		}
	}
	
	File->Close();
}

void UChallengeFunctionLibrary::BRC_TSI_PAS(const FString& InInputName)
{
	constexpr int billion = 1000000000;

	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UChallengeFunctionLibrary::BRC_TSI"));
	// open file
	const FString Path = FPaths::ProjectContentDir() + TEXT("Data/") + InInputName;
	TUniquePtr<FArchive> File(IFileManager::Get().CreateFileReader(*Path));
	
	// Prepare RET
	TMap<FString, int> Indices;
	TArray<FBRCStruct_Naive> Rows;
	Rows.Reserve(billion);
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Reading data"));
		
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
			
				if(int* index = Indices.Find(Station))
				{
					FBRCStruct_Naive& row = Rows[*index];
					row.min = row.min < candidate ? row.min : candidate;
					row.max = row.max > candidate ? row.max : candidate;
					row.avg = (row.avg * row.count + candidate) / (row.count + 1);
				}
				else
				{
					const int currIndex = Rows.Num();
					Rows.Add(FBRCStruct_Naive(candidate));
					Indices.Add(Station, currIndex);
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
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Sorting data"));
		Indices.GetKeys(SortedKeys);
		SortedKeys.Sort();
	}
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("BRC_TSI::Printing data"));
		for (auto key : SortedKeys)
		{
			int* index = Indices.Find(key);
			FBRCStruct_Naive& row = Rows[*index];
			UE_LOG(LogTemp, Error, TEXT("%s;%.1f;%.1f;%.1f"), *key, row.min, row.avg, row.max);
		}
	}
	
	File->Close();
}