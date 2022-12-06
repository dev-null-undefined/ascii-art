# ascii-art

> ascii-art terminal based image viewer
> with zoom,move and color capabilities

## :computer: Running using nix package manager
This requires [flake](https://nixos.wiki/wiki/Flakes) experimental commands
```
# Create shell environment with the ascii-art package
nix shell github:dev-null-undefined/ascii-art --no-write-lock-file

# Or you can just run it
nix run github:dev-null-undefined/ascii-art --no-write-lock-file
```

## :cloud: Installation from sources

```sh
# Clone the repository
git clone https://github.com/dev-null-undefined/ascii-art

# Change directory
cd ascii-art

# Compile binary 2 options
# A) Using nix
nix build .#

# B) Using make file
make compile

# Start the program
./bin/ascii-art
# or if using nix
./result/bin/ascii-art
```

## :clipboard: Example

```sh
./bin/ascii-art ~/Downloads # Load all images from download folder and open them using ascii-art program
./bin/ascii-art # open regex file menu to search for files to open
```

## :question: Get Help

There are few ways to get help:

1. For bug reports and feature requests, open issues. :bug:
2. [post questions on Stack Overflow](https://stackoverflow.com/questions/ask). You can open issues with questions, and
   add link to your Stack Overflow question.

## :memo: Documentation

Code documentation is available on [github-pages](https://dev-null-undefined.github.io/ascii-art/)
