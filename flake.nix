{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    devshell.url = "github:numtide/devshell/main";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    devshell,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
        overlays = [devshell.overlays.default];
      };
      libraries = with pkgs; [
        libjpeg_original
        ncurses
        libpng12
        zlib
      ];
      compiler = pkgs.gcc;
      dev-deps = with pkgs; [glibc libcxx doxygen graphviz];
      package-config = rec {
        pname = "ascii-art";
        packages-name = pname;
        version = "v0.0.0";
        src = pkgs.fetchFromGitHub {
          owner = "dev-null-undefined";
          repo = pname;
          rev = version;
          sha256 = "sha256-dQJjBH0gxn8FBMyyC9DRYOergOUzns/+jUNJ1KSVTtk=";
        };
      };
      packages = {
        ${package-config.packages-name} = pkgs.stdenv.mkDerivation rec {
          inherit (package-config) pname version src;

          buildInputs = libraries ++ [pkgs.doxygen];

          installPhase = ''
            mkdir -p $out/bin
            mv bin/${pname} $out/bin/
          '';

          meta = with pkgs.lib; {
            homepage = "";
            description = "";
            longDescription = "";
            platforms = platforms.linux;
          };
        };
        default = self.packages.${system}.${package-config.packages-name};
      };
      default-app = {
        type = "app";
        program = self.packages.${system}.default + "/bin/${package-config.pname}";
      };
    in {
      apps.default = default-app;
      devShell = pkgs.devshell.mkShell {
        name = package-config.pname;
        imports = ["${devshell}/extra/language/c.nix"];
        packages = dev-deps;

        language.c = {
          inherit compiler libraries;
          includes = libraries;
        };
        commands = [
          {
            name = "compile";
            category = "c++";
            help = "Optimized compilation with nearly non logging";
            command = "${pkgs.gnumake}/bin/make fast";
          }
          {
            name = "debug";
            category = "c++";
            help = "Faster compilation with more debug info and logging";
            command = "${pkgs.gnumake}/bin/make fast-debug";
          }
          {
            name = "clean";
            category = "c++";
            help = "Clean binaries (Run this between all debug and release versions)";
            command = "${pkgs.gnumake}/bin/make clean";
          }
          {
            name = "run";
            category = "testing";
            command = "${pkgs.gnumake}/bin/make run";
          }
          {
            name = "docs";
            category = "c++";
            help = "Generate documentation";
            command = "${pkgs.gnumake}/bin/make docs";
          }
        ];
        bash = {
          extra = ''
            export CPLUS_INCLUDE_PATH="$C_INCLUDE_PATH"
            export LIBRARY_PATH="$LD_LIBRARY_PATH"
          '';
        };
      };

      defaultPackage = self.packages.${system}.default;

      inherit packages;

      formatter = pkgs.alejandra;

      cmake-helper = rec {
        libs = builtins.map builtins.toString (builtins.map pkgs.lib.getLib libraries);
        includes = builtins.map builtins.toString (builtins.map pkgs.lib.getDev libraries);
        cmake-file = pkgs.writeText "CMakeList.txt" (pkgs.lib.strings.concatLines (
          (builtins.map (lib: ''target_link_directories(''${CMAKE_PROJECT_NAME} PUBLIC ${lib}/lib)'') libs)
          ++ (builtins.map (include: ''include_directories(${include}/include)'') includes)
        ));
      };
    });
}
