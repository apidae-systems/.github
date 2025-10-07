#esp-generate esp-rust --chip esp32s3 -o unstable-hal -o alloc -o embassy -o esp-backtrace -o defmt -o wokwi -o dev-container -o ci -o helix -o vscode -o zed -o ble-trouble -o wifi -O .
{
  languages.rust = {
    enable = false;
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
