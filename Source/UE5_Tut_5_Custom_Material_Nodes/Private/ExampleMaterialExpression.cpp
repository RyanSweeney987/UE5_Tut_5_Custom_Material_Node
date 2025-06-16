// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleMaterialExpression.h"

#include "MaterialCompiler.h"

UExampleMaterialExpression::UExampleMaterialExpression()
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

	ExampleAInput.InputName = "Example Rename of Pin";
	
	// Show/hide pins on the node
	bShowInputs = true;
	bShowOutputNameOnPin = true;
	bShowMaskColorsOnPin = false;

	// Setup output pins
	Outputs.Reset();
	Outputs.Add(FExpressionOutput(TEXT("Result"), true, true, true, true, true));
#endif
}

// Not sure if this is needed, but you can find this macro before the Compile function in many Unreal Engine material expression source files.
#if WITH_EDITOR
/**
 * Compile the material expression.
 * This function is called by the Material Editor to generate the shader code for this expression.
 * @param Compiler The material compiler used to compile the expression.
 * @param OutputIndex The index of the output pin being compiled.
 * @return The index of the compiled output.
 */
int32 UExampleMaterialExpression::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	// Set the input value to the set values of ExampleA, or the value from the input pin if it's connected
	int32 InputValue = Compiler->Constant4(ExampleA.X, ExampleA.Y, ExampleA.Z, ExampleA.W);
	if(ExampleAInput.GetTracedInput().Expression)
	{
		InputValue = ExampleAInput.Compile(Compiler);
	}

	// This is an example of how to throw errors in the material expression
	// return Compiler->Error(TEXT("This is an example error"));

	// Get each colour
	const int32 Red = Compiler->ComponentMask(InputValue, true, false, false, false);
	const int32 Green = Compiler->ComponentMask(InputValue, false, true, false, false);
	const int32 Blue = Compiler->ComponentMask(InputValue, false, false, true, false);
	const int32 Alpha = Compiler->ComponentMask(InputValue, false, false, false, true);

	// Recombine them in the order of Alpha, Blue, Green, Red
	int32 Result = Compiler->AppendVector(Alpha, Blue);
	Result = Compiler->AppendVector(Result, Green);
	Result = Compiler->AppendVector(Result, Red);
	
	return Result;
}

int32 UExampleMaterialExpression::CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	return Super::CompilePreview(Compiler, OutputIndex);
}

/**
 * Get the caption for the material expression.
 * @param OutCaptions The array to fill with captions.
 */
void UExampleMaterialExpression::GetCaption(TArray<FString>& OutCaptions) const
{
	// Name for the node
	OutCaptions.Add(TEXT("Example Material Expression"));
}

#endif
