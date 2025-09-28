{ config, pkgs, ... }:
{
  scripts = {
    hello = {
      description = "  👋 Show a friendly greeting";
      packages = with pkgs; [ chafa ];
      exec = ''
        chafa --align center "${config.env.DEVENV_ROOT}/public/apidae-systems-banner.png"
        echo "👋🧩"
      '';
    };

    clean = {
      exec = "git clean -fdX; pnpm i; doctor";
    };

    doctor = {
      packages = with pkgs; [
        figlet
        shellspec
      ];

      description = " 💊 Run Microdoctor health-check suite with docs output";
      exec = ''
        figlet -cf slant "💊 Microdoctor";
        shellspec -c "${config.env.DEVENV_ROOT}/apps/microvisor" --quiet "$@";
      '';
    };

    kernel = {
      description = " 🎉 Fire up the Microvisor Kernel";
      exec = ''
        devenv up
      '';
    };

    "dev:api" = {
      description = "    🧩 Start the api dev server";
      exec = "nx dev api";
    };

    "test:api" = {
      description = "    🚀 Start the test run with any args passed through";
      exec = "nx test api";
    };

    graph = {
      description = "  📍 Generate an Nx dependency graph, grouped by folder";
      exec = ''
        nx graph --groupByFolder "$@"
      '';
    };
  };
}
