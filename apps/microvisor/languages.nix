{
  pkgs,
  ...
}:
{
  languages = {
    nix.enable = true;
    shell.enable = true;
    typescript.enable = true;
    javascript = {
      enable = true;
      pnpm.enable = true;
      package = pkgs.nodejs_24;
    };
  };
}
