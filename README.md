# GlowControl for LIFX®

GlowControl is a super simple Ubuntu Touch app for LIFX®.

## Installation

Get it from the Ubuntu (Click) App store.

## Usage

Make sure you're on the same network as your LIFX® bulbs.

## Contributing

1. Fork it!
1. Create your feature branch: `git checkout -b my-new-feature`
1. Commit your changes: `git commit -am 'Add some feature'`
1. Rebase your commits.
1. Push to the branch: `git push origin my-new-feature`
1. Submit a pull request

## Build from source

Install submodules:

    git submodule update --init --recursive

Build depends on 

    cmake
    qtbase5-dev qtdeclarative5-dev intltool

Build it:

    Install the deps
    mkdir build && cd build
    cmake ../
    make

Make a click:

    mkdir click-build && cd build-click
    cmake -DCLICK_MODE:BOOLEAN=1 ../
    make DESTDIR=out install
    click build out


## Credits

Author: Jonas G. Drange

Thanks to Andrew Kane for lib-lifx.


## License

MIT, see LICENSE.
