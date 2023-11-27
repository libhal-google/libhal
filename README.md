# libhal

[![✅CI](https://github.com/libhal/libhal/actions/workflows/ci.yml/badge.svg)](https://github.com/libhal/libhal/actions/workflows/ci.yml)
[![coverage](https://libhal.github.io/libhal/coverage/coverage.svg)](https://libhal.github.io/libhal/coverage/)
[![GitHub stars](https://img.shields.io/github/stars/libhal/libhal.svg)](https://github.com/libhal/libhal/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/libhal/libhal.svg)](https://github.com/libhal/libhal/network)
[![GitHub issues](https://img.shields.io/github/issues/libhal/libhal.svg)](https://github.com/libhal/libhal/issues)

The foundation of the libhal ecosystem, containing the core hardware interfaces
used to make the whole ecosystem work.

## 📚 Software APIs & Usage

To learn about the available drivers and APIs see the
[API Reference](https://libhal.github.io/2.2/api/namespacehal/)
documentation page or look at the
[`include/libhal`](https://github.com/libhal/libhal/tree/main/include/libhal)
directory.

## 🧰 Setup

Following the
[🚀 Getting Started](https://libhal.github.io/2.2/getting_started/)
instructions.

## 📦 Adding `libhal` to your project

This section assumes you are using the
[`libhal-starter`](https://github.com/libhal/libhal-starter)
project.

Add the following to your `requirements()` method to the `ConanFile` class:

```python
    def requirements(self):
          self.requires("libhal/[^2.0.3]", transitive_headers=True)
```

The version number can be changed to whatever is appropriate for your
application. If you don't know, using the latest is usually a good choice.

Normally, this is not done, as usually libhal is into the code base via
implementation libraries such `libhal-lpc40` and `libhal-stm32f1` for
peripheral drivers, or, `libhal-esp8266` and `libhal-mpu` for device drivers.

## :busts_in_silhouette: Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.

## Disclaimer

This project is not an official Google project. It is not supported by
Google and Google specifically disclaims all warranties as to its quality,
merchantability, or fitness for a particular purpose.
