using UnrealBuildTool;

public class UE_PortfolioSettings : ModuleRules
{
    public UE_PortfolioSettings(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            // Initial Modules
            "Core", "CoreUObject", "Engine", "InputCore",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

    }
}