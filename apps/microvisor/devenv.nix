{
  lib,
  pkgs,
  config,
  inputs,
  ...
}:

let
  CONSTANTS = rec {
    APIDAE_SYSTEMS = {
      DOMAIN = "apidaesystems.ca";
      GITHUB = "github.com/apidae-systems";
      LINKEDIN = "linked.com/company/apidae-systems";
    };

    SUPABASE = {
      DOMAIN = "supabase.co";
    };
  };

  PORTS = {
    WEB = "3000";
    API = "5150";
    DOCS = "5173";
    GRAPH = "4211";
    ADMIN = "8000";
    POSTGRES = "54322";
  };

  URLS = rec {
    SCHEME = "http";
    HOST = "127.0.0.1";
    BASE = CONSTANTS.APIDAE_SYSTEMS.DOMAIN;
    LOCALHOST = "${SCHEME}://${HOST}:";
  };

  # FLAGS = {
  #   DEVELOPMENT = true;
  # };
in
{
  imports = [
    ./cachix.nix
    ./files.nix
    ./languages
    # ./git-hooks.nix
    ./scripts.nix
  ];

  env = rec {
    ZELLIJ_AUTO_EXIT = "true";
    ZELLIJ_AUTO_ATTACH = "true";
    NX_VERBOSE_LOGGING = "true";
    #====================================================
    #                    🏁 FLAGS
    #====================================================
    # PRODUCTION = !FLAGS.DEVELOPMENT;

    #====================================================
    #                    🔌 PORTS
    #====================================================
    WEB_SERVER_PORT = "${PORTS.WEB}";
    API_SERVER_PORT = "${PORTS.ADMIN}";
    ADMIN_SERVER_PORT = "${PORTS.ADMIN}";

    #====================================================
    #                     🔗 URLS
    #====================================================
    BASE_URL = "${URLS.BASE}";
    WEB_LOCAL_URL = "${URLS.LOCALHOST}${PORTS.WEB}";
    ADMIN_LOCAL_URL = "${URLS.LOCALHOST}${PORTS.ADMIN}";

    CORS_WHITELIST_ORIGINS = "${ADMIN_LOCAL_URL},${ADMIN_LOCAL_URL}";
    CSRF_WHITELIST_ORIGINS = "${ADMIN_LOCAL_URL},${ADMIN_LOCAL_URL}";
  };

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
    ]
    ++ lib.optionals (!(config.container.isBuilding or false)) [
      glab
    ];

  process = {
    manager.args."theme" = "One Dark";
    managers.process-compose = {
      tui.enable = true;
      settings = {
        environment = [
          "NX_TUI = false"
        ];
        availability = {
          max_restarts = 5;
          backoff_seconds = 2;
          restart = "on_failure";
        };
      };
    };
  };

  files = {
    ".shellspec" = {
      text = "";
    };
  };

  processes = {
    doctor = {
      exec = "doctor";
      process-compose = {
        disabled = true;
        working_dir = "./";
        namespace = "💊 HEALTHCHECK";
      };
    };

    "dev:api" = {
      exec = "dev:api";
      process-compose = {
        disabled = false;
        working_dir = "./";
        namespace = "🧩 API";
        description = "🧩 API | ${PORTS.API}";
        depends_on = {
          redis.condition = "process_healthy";
        };
        readiness_probe = {
          period_seconds = 10;
          timeout_seconds = 5;
          success_threshold = 1;
          failure_threshold = 3;
          initial_delay_seconds = 1;
          http_get = {
            path = "/v0/health";
            host = URLS.HOST;
            scheme = URLS.SCHEME;
            port = builtins.fromJSON PORTS.ADMIN;
          };
        };
      };
    };

    "build:api" = {
      exec = "nx build api";
      process-compose = {
        disabled = true;
        working_dir = "./";
        namespace = "🏗 BUILD";
        depends_on = {
          doctor.condition = "process_completed_successfully";
        };
      };
    };

    "test:api" = {
      exec = "nx test api";
      process-compose = {
        disabled = true;
        working_dir = "./";
        namespace = "🔬 UNIT";
        depends_on = {
          "build:api".condition = "process_completed_successfully";
        };
      };
    };

    "e2e:api" = {
      exec = "nx e2e api";
      process-compose = {
        disabled = true;
        working_dir = "./";
        namespace = "🎭 E2E";
        depends_on = {
          "build:api".condition = "process_completed_successfully";
        };
      };
    };

    "graph" = {
      exec = "nx graph --view=projects --affected --port ${PORTS.GRAPH}";
      process-compose = {
        is_tty = true;
        disabled = true;
        namespace = "🧮 VIEWS";
        description = "📍 Project Graph - Nx | ${PORTS.GRAPH} ";
        readiness_probe = {
          http_get = {
            host = URLS.HOST;
            scheme = URLS.SCHEME;
            port = builtins.fromJSON PORTS.GRAPH;
          };
        };
      };
    };

    "dev:docs" = {
      exec = "nx dev docs";
      process-compose = {
        disabled = true;
        namespace = "🧮 VIEWS";
        # is_foreground = true; # requires process to be manually started via 'ctrl+R'
        description = "📚 DOCS | ${PORTS.DOCS}";
      };
    };
  };

  enterShell = ''
    hello
  '';

  tasks = {
    "build:api" = {
      # status = "echo hello";
      exec = "nx build api";
      before = [ "devenv:processes:test:api" ];
      # execIfModified = [
      #   "src/**/*.ts"  # All TypeScript files in src directory
      #   "*.json"       # All JSON files in the current directory
      #   "package.json" # Specific file
      #   "src"          # Entire directory
      # ];
    };
  };

  enterTest = ''
    nx run-many -t build test e2e --parallel
  '';
}
