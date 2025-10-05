{
  pkgs,
  ...
}:
{
  packages =
    with pkgs;
    [
      binsider

      espup
      esptool
      esphome
      espflash
      probe-rs
      esp-generate
      cargo-espmonitor
      cp210x-program # needed for macOS serial connection to ESP32

      ninja
      ccache
      dfu-util

      pulumi-esc

      binaryen
      wasm-pack
      dioxus-cli
      # NOTE: nixpkgs only has up to v100, need 104 for dioxus
      # HACK: cargo install wasm-bindgen-cli
      # wasm-bindgen-cli
    ]
    ++ lib.optionals (!(config.container.isBuilding or false)) [
      glab
    ];
}
