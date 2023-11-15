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
      libs = with pkgs; [
        libjpeg_original
        ncurses
        libpng12
        zlib
      ];
      program-name = "ascii-art";
    in {
      apps = {
        default = {
          type = "app";
          program = self.packages.${system}.${program-name} + "/bin/${program-name}";
        };
      };

      defaultApp = {
        type = "app";
        program = self.packages.${system}.${program-name} + "/bin/${program-name}";
      };

      devShell = pkgs.devshell.mkShell {
        name = "${program-name}";
        imports = ["${devshell}/extra/language/c.nix"];
        packages = with pkgs; [gcc glibc libcxx doxygen graphviz];

        language.c = {
          libraries = libs;
          includes = libs;
          compiler = pkgs.gcc;
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

      defaultPackage = self.packages.${system}.${program-name};

      packages = {
        ${program-name} = let
          pname = "${program-name}";
          version = "v0.1.2";
        in
          pkgs.stdenv.mkDerivation {
            inherit pname version;
            src = pkgs.fetchFromGitHub {
              owner = "dev-null-undefined";
              repo = pname;
              rev = version;
              sha256 = "sha256-dQJjBH0gxn8FBMyyC9DRYOergOUzns/+jUNJ1KSVTtk=";
              fetchSubmodules = true;
            };

            buildInputs = libs ++ [pkgs.doxygen];

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

        default = self.packages.${system}.${program-name};
      };

      formatter = pkgs.alejandra;
      cmake-helper = let
        libs' = builtins.map builtins.toString (builtins.map pkgs.lib.getLib libs);
        includes' = builtins.map builtins.toString (builtins.map pkgs.lib.getDev libs);
      in {
        libs = libs';
        includes = includes';
        cmake-file = pkgs.writeText "CMakeList.txt" (pkgs.lib.strings.concatLines (
          (builtins.map (lib: ''target_link_directories(''${CMAKE_PROJECT_NAME} PUBLIC ${lib}/lib)'') libs')
          ++ (builtins.map (include: ''include_directories(${include}/include)'') includes')
        ));
      };
    });
}
