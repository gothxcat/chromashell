# ChromaShell

Display lines of color in a true color terminal.

## Usage

`$ chromashell -h`

### Example: Display a transgender flag

`$ chromashell -p trans`

Or:

`$ chromashell -s 7ACBF5,2 -s EAACB8,2 -s FFFFFF,2 -s EAACB8,2 -s 7ACBF5,2`

## Runtime requirements

- POSIX-compliant operating system
- Terminal with true color support
- `cjson`
- `gettext`

## Build requirements

- `cjson`
- `cmake` >= 3.1
- `gcc`
- `gettext`

## Building

### Release

`$ (mkdir -p build && cd build && cmake .. && make)`

### Debug

`$ (mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug && make)`

## Installation

`$ (cd build && make install)`

## Uninstallation

`$ (cd build && make uninstall)`

## Documentation

See [`doc/`](doc).

## License

Released under the GNU GPLv3.

See `LICENSE` for more information.

## Acknowledgements

Inspired by [cli-pride-flags](https://github.com/ExperiBass/cli-pride-flags) <3