# A very annoying lint error.

Recently, I am getting learning about Linux and Cuda. I am using archlinux which is a rolling release distro, so every package on my system is the latest version.

When I develop Cuda code using VSCode, I often get the following lint error:

![](./figs/2025-11-10_22-33.png)

This is a very strange error. How can such a simple insertion operator `<<` cause an error?

Althouth the code can still be compiled and run correctly, this lint error is very annoying.

I have searched online for a long time but found no solution. Finally, I found the root cause of this problem.

After the CUDA toolkit move to version 13, NVIDIA quietly removed the legacy header `texture_fetch_functions.h` from `targets/x86_64-linux/include`. Clang, however, still ships its CUDA runtime wrapper (`/usr/lib/clang/21/include/__clang_cuda_runtime_wrapper.h`) that unconditionally does `#include "texture_fetch_functions.h"` at the end of every CUDA translation unit. When clangd replays the compile command for semantic analysis, the wrapper tries to pull in that header, cannot find it, and aborts the parse before it gets to `<iostream>`. Because the parse stops so early, none of the built-in CUDA keywords (e.g. `__global__`) or the standard library aliases (like `std::ostream`) are registered, which is why clangd ends up claiming that `std::cout` is an `int` and `<<` is invalid.

However, a very simple solution is to include the missing header file manually. By adding a empty header file `texture_fetch_functions.h` somewhere in the include path, the lint error disappears.

See my `.clangd` file and the empty header file in `./include_oldcuda` in this repository for details.