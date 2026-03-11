{
    description = "Derivation for the pajeng library";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
        flake-utils.url = "github:numtide/flake-utils";
    };

    outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
        let
            pkgs = import nixpkgs { inherit system; };
        in
        {
            packages.pajeng = pkgs.stdenv.mkDerivation (finalAttrs: {
                pname = "pajeng";
                version = "1.3.10";
                inherit system;

                src = ./.;

                nativeBuildInputs = with pkgs; [
                    perl
                    cmake 
                    boost
                    asciidoc
                    flex
                    bison
                    fmt
                    patchelf
                ];

                cmakeFlags = [ 
                    # "-DCMAKE_POLICY_VERSION_MINIMUM=3.5" 
                    # Tells CMake to use the final installation RPATH even during the build phase
                    "-DCMAKE_BUILD_WITH_INSTALL_RPATH=ON"
                    # Prevents CMake from adding the build directory to the RPATH
                    "-DCMAKE_SKIP_BUILD_RPATH=OFF" 
                    # Ensure the install RPATH is actually set to the Nix store lib path
                    "-DCMAKE_INSTALL_RPATH=${placeholder "out"}/lib"
                    "-DCMAKE_BUILD_TYPE=Release"
                ];

                # esse aqui foi dica do amiguinho
                # não entendi porque funciona exatamente
                # segundo ele, no check a lib não está ainda no path final na store
                # devido as flags passadas ao cmake
                # então fazemos essa inserção manual para que os testes funcionem
                preCheck = ''
                    export LD_LIBRARY_PATH=$PWD/src/libpaje:$LD_LIBRARY_PATH
                '';

                postInstall = ''
                    patchelf --print-rpath "$out/bin/pj_dump"
                '';

                doCheck = true;
            });


            defaultPackage = self.packages.${system}.pajeng;
            devShells.default = pkgs.mkShell {
                buildInputs = [ self.packages.${system}.pajeng ];
            };
        }
    );
}
