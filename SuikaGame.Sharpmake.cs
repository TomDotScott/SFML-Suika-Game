using System.IO;
using Sharpmake;

[Generate]
public class SuikaGame : Project
{
    public SuikaGame()
    {
        Name = "SuikaGame";

        SourceRootPath = "src";

        AddTargets(
            new Target(
                Platform.win64,
                DevEnv.vs2022,
                Optimization.Debug |
                Optimization.Release |
                Optimization.Retail
            )
        );
    }

    [Configure]
    public void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.ProjectPath = $"build_{target.DevEnv}";
    }
}

[Generate]
public class SuikaGameSolution : Solution
{
    public SuikaGameSolution()
    {
        Name = "SFML_Suika_Game";

        AddTargets(
            new Target(
                Platform.win64,
                DevEnv.vs2022,
                Optimization.Debug |
                Optimization.Release |
                Optimization.Retail
            )
        );
    }

    [Configure]
    public void ConfigureAll(Solution.Configuration conf, Target target)
    {
        conf.SolutionPath = "[solution.SharpmakeCsPath]";

        conf.AddProject<SuikaGame>(target);
    }
}

