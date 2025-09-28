{
  pkgs,
  ...
}:
{
  packages =
    with pkgs;
    [
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

      dioxus-cli
    ]
    ++ lib.optionals (!(config.container.isBuilding or false)) [
      glab
    ];
}
