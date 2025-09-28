{
  languages.rust = {
    enable = true;
    channel = "nightly";
    targets = ["wasm32-unknown-unknown"];
    components = [
      "rustc"
      "cargo"
      "clippy"
      "rustfmt"
      "rust-src"
      "rust-std"
      "rust-analyzer"
    ];
  };
}
