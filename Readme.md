# Logic
[![Build Status](https://travis-ci.com/udeyrishi/Logic.svg?token=ez7psV6XvuSyQ3hU3b5M&branch=master)](https://travis-ci.com/udeyrishi/Logic)

Logic is a Boolean Function manipulation library + a shell for performing these operations. It's in a really early development stage, so the exact details are not sorted out yet. But come back here later to see the updates!

##Building

Requirements:

* clang
* cmake

This project is only compatible with `clang` (tested on v3.6 - v3.8 on Linux and clang-800.0.38 on macOS). `g++` complains for some reason, because of some weird linking issues. If you really _really_ need g++ support, please file an issue.


```sh
cmake . && make
```

##Running
To launch the shell, go:

```
./bin/logic
```
![demo2](https://cloud.githubusercontent.com/assets/4692593/18813438/a10825f4-82ae-11e6-92e9-31cf4da269cd.gif)

You can also logic for interpreting a source file:
```
./bin/logic <path to file>
```
![demo](https://cloud.githubusercontent.com/assets/4692593/18813400/a72fbd8a-82ad-11e6-888a-1f16806a74fd.gif)
