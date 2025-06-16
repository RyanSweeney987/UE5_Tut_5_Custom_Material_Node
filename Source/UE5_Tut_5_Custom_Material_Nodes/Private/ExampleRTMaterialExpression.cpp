// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleRTMaterialExpression.h"

#include "MaterialCompiler.h"

// Some info was found here
// https://forums.unrealengine.com/t/custom-material-node-to-extract-utexture2d-data-in-c/552609/4?u=sweenus987
// https://unreal.shadeup.dev/docs/nodes


UExampleRTMaterialExpression::UExampleRTMaterialExpression()
{
	// This is used for the add menu category in the Material Editor
	struct FConstructorStatics
	{
		FText MENU_CATEGORY; 
		// This is used for placing the expression in the correct category
		// You can reference multiple categories here, see: https://github.com/EpicGames/UnrealEngine/blob/ue5-main/Engine/Source/Runtime/Engine/Private/Materials/MaterialExpressions.cpp#L18670
		FConstructorStatics()
			: MENU_CATEGORY(NSLOCTEXT( "ExampleNamespace", "Example", "Example Material Expressions" )) // These can be anything. Like: NAME_Math(LOCTEXT( "Math", "Math" ))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;


#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.MENU_CATEGORY);

	TextureInput.InputName = "Texture Input";
	
	// Show/hide pins on the node
	bShowInputs = true;
	bShowOutputNameOnPin = true;
	bShowMaskColorsOnPin = false;

	// Setup output pins
	Outputs.Reset();
	Outputs.Add(FExpressionOutput(TEXT("Result"), true, true, true, true, true));
#endif
}

int32 UExampleRTMaterialExpression::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	UMaterialExpression* InputExpression = TextureInput.GetTracedInput().Expression;
	
	if (Texture || InputExpression)
	{
		int32 TextureCodeIndex = INDEX_NONE;
		EMaterialSamplerType Sampler = EMaterialSamplerType::SAMPLERTYPE_Color;

		if (InputExpression)
		{
			// Get the texture from the texture object node
			TextureCodeIndex = TextureInput.Compile(Compiler);
			// Get the actual texture object so we can get the sampler type
			UObject* InTex = TextureInput.Expression->GetReferencedTexture();
			Sampler = UMaterialExpressionTextureBase::GetSamplerTypeForTexture(Cast<const UTexture>(InTex));
		}
		else
		{
			// Get the sampler from the texture input
			Sampler = UMaterialExpressionTextureBase::GetSamplerTypeForTexture(Texture);
			constexpr ESamplerSourceMode SamplerSource = SSM_FromTextureAsset;

			// Get the texture
			int32 TextureRef = INDEX_NONE;
			TextureCodeIndex = Compiler->Texture(Texture, TextureRef, Sampler, SamplerSource, TMVM_None);
		}

		// Get the texture coordinate
		const int32 TextureCoordinate = Compiler->TextureCoordinate(0, false, false);	
		// Sample the texture
		const int32 SampleTexture = Compiler->TextureSample(TextureCodeIndex, TextureCoordinate, Sampler);
		// Get each colour
		const int32 Red = Compiler->ComponentMask(SampleTexture, true, false, false, false);
		const int32 Green = Compiler->ComponentMask(SampleTexture, false, true, false, false);
		const int32 Blue = Compiler->ComponentMask(SampleTexture, false, false, true, false);
		const int32 Alpha = Compiler->ComponentMask(SampleTexture, false, false, false, true);

		// Recombine them in the order of Alpha, Blue, Green, Red
		int32 Result = Compiler->AppendVector(Alpha, Blue);
		Result = Compiler->AppendVector(Result, Green);
		Result = Compiler->AppendVector(Result, Red);
		
		return Result;
	}

	return Compiler->Error(TEXT("No texture or input expression connected to ExampleRTMaterialExpression"));
}

int32 UExampleRTMaterialExpression::CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	return Super::CompilePreview(Compiler, OutputIndex);
}

void UExampleRTMaterialExpression::GetCaption(TArray<FString>& OutCaptions) const
{
	// Name for the node
	OutCaptions.Add(TEXT("Example Texture Material Expression"));
}

/*
 * Since we only have one input, we can set the type directly.
 */
uint32 UExampleRTMaterialExpression::GetInputType(int32 InputIndex)
{
	return MCT_Texture;
}
