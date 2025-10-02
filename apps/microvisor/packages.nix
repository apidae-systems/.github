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
      esp-generate
      probe-rs

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
