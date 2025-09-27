{ pkgs, ... }:

{
  packages = with pkgs; [ glab ];

  languages={
    nix.enable = true;
    shell.enable = true;
    typescript.enable = true;
    javascript={
      enable = true;
      pnpm.enable = true;
      package = pkgs.nodejs_24;
      };
    };

  enterShell = ''
    echo hellooo
  '';

  enterTest = ''
    echo "Running tests"
    git --version | grep --color=auto "${pkgs.git.version}"
  '';
}
