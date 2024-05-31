# rpi-pico-startup
Generic Raspberry Pico startup code

## Getting Started

1. [Dependencies](#dependencies)
2. [Build](#build)
   1. [Compilation](#compilation)
   2. [Distribution package](#distribution-package)

### Dependencies
Artifact               | Version | Repository
-----------------------|---------|-----------
rpi-pico-hardware-regs | v1.1.0  | https://github.com/jmarcelocruz/rpi-pico-hardware-regs

For each dependency, get the release artifact and extract it under `external/`.

### Build
This project is built with Make.

#### Compilation

Compile each of the project's object files with `make <object>.o`.

The following variables are available to control the compilation step:
- `CC`, sets the C compiler used
- `CFLAGS`, overrides all flags passed to the C compiler
- `EXTRA_CFLAGS`, appends to the default flags passed to the C compiler

#### Distribution package

Generate the distribution package for the current version with `make dist` or simply `make`.

The version number can be overriden temporarily by passing variables `VERSION_MAJOR`, `VERSION_MINOR` and `VERSION_PATCH`. Once a new release is ready, update these variables in the `Makefile`.
