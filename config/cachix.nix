{
  cachix = {
    enable = true;
    push = "mfarabi";
    pull = [
      "devenv"
      "cachix"
      "oxalica"
      "nixpkgs"
      "mfarabi"
      "nix-darwin"
      "nix-community"
      "apidae-systems"
      "pre-commit-hooks"
    ];
  };
}
