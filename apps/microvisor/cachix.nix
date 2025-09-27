{
  cachix = {
    enable = true;
    push = "mfarabi";
    pull = [
      "devenv"
      "cachix"
      "nixvim"
      "oxalica"
      "nixpkgs"
      "mfarabi"
      "apidae-systems"
      "emacs-ci"
      "nix-darwin"
      "nix-community"
      "pre-commit-hooks"
    ];
  };
}
