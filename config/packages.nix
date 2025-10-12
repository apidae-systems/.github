{
  pkgs,
  ...
}:
{
  packages =
    with pkgs;
    [
      binsider # binary inspector TUI

      # embuild
      # cargo-pio
      platformio # nixos.wiki/wiki/Platformio

      espup
      esptool
      esphome
      espflash
      # probe-rs # connect to debug probes
      esp-generate
      cargo-espmonitor
      # cp210x-program # needed for macOS serial connection to ESP32

      # python313Packages.atopile # circuit boards as code

      tinygo
      openocd # Flash STM32
      avrdude # flash Arduinos

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
