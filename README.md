# asciify

An attempt at making a program that converts images to ASCII characters.

## Usage

```
Usage: build/asciify [options ...] file
  -h, --help            show this information
  -c, --chars <string>  set output characters from dark to light
  -s, --scale <float>   set scaling of the image and text output
  -C, --colorize        colorize text output, uses true color (24-bit color)
```

## Example

Below is the result of converting [this](assets/example.png) image into a colored ASCII art.

![example-output](assets/example-output.jpg)

## Building

### Prerequisites

* GNU Make
* GCC (or any C compiler that works)

Type `make` in the root directory and the executable file should be in the `build/` directory.

## License

MIT
