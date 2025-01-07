using Sharpmake;

[module: Sharpmake.Include("SuikaGame.Sharpmake.cs")]

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments arguments)
    {
        arguments.Generate<SuikaGameSolution>();
    }
}