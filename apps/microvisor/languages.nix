{
  pkgs,
  ...
}:
{
  languages = {
    nix.enable = true;
    shell.enable = true;
    cplusplus.enable = true;
    typescript.enable = true;
    c = {
      enable = true;
      debugger = pkgs.gdb;
    };
    javascript = {
      enable = true;
      pnpm.enable = true;
      package = pkgs.nodejs_24;
    };
  };
}
