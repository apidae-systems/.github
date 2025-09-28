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
    rust = {
     enable = true;
     channel = "nightly";
      components = [
        "rustc"
        "cargo"
        "clippy"
        "rustfmt"
        "rust-src"
        "rust-analyzer"
      ];
    };
    javascript = {
      enable = true;
      pnpm.enable = true;
      package = pkgs.nodejs_24;
    };
  };
}
