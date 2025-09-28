{
  pkgs,
  ...
}:
{
    languages.javascript = {
      enable = true;
      pnpm.enable = true;
      package = pkgs.nodejs_24;
    };
}
