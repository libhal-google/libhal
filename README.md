<div align="center">
<img height="250" src="https://raw.githubusercontent.com/libhal/.github/main/profile/logo.png">
<br />
<br />

```
🚚 portable 🦾 flexible 📦 accessible 🍰 easy to use
```
<br />
</div>

<div align="center">

[![✅CI](https://github.com/libhal/libhal/actions/workflows/ci.yml/badge.svg)](https://github.com/libhal/libhal/actions/workflows/ci.yml)
[![coverage](https://libhal.github.io/libhal/coverage/coverage.svg)](https://libhal.github.io/libhal/coverage/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b084e6d5962d49a9afcb275d62cd6586)](https://www.codacy.com/gh/libhal/libhal/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=libhal/libhal&amp;utm_campaign=Badge_Grade)
[![GitHub stars](https://img.shields.io/github/stars/libhal/libhal.svg)](https://github.com/libhal/libhal/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/libhal/libhal.svg)](https://github.com/libhal/libhal/network)
[![GitHub issues](https://img.shields.io/github/issues/libhal/libhal.svg)](https://github.com/libhal/libhal/issues)
[![Latest Version](https://libhal.github.io/libhal/latest_version.svg)](https://github.com/libhal/libhal/blob/main/conanfile.py)
[![ConanCenter Version](https://repology.org/badge/version-for-repo/conancenter/libhal.svg)](https://conan.io/center/libhal)

</div>

## [:books: Interface APIs](https://libhal.github.io/libhal/api)

## :inbox_tray: Install

### `conanfile.txt`
Add the following line to your `conanfile.txt` file under the `[requires]`
section:

```
libhal/[x]
```

Replace `[x]` with the appropriate version number.

### `conanfile.py`

Add the following to your `def requirements(self):` function.

```python
self.requires("libhal/[x]")
```

Replace `[x]` with the appropriate version number.

## :busts_in_silhouette: Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.

## Disclaimer

This project is not an official Google project. It is not supported by
Google and Google specifically disclaims all warranties as to its quality,
merchantability, or fitness for a particular purpose.
