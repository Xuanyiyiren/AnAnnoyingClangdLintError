# 一个非常恼人的 `clangd` lint 错误

最近我在学习 Linux 和 CUDA。我使用的是 Arch Linux，这是一款滚动发行版，所以系统里的所有软件包都是最新版本。

当我在 VSCode 中开发 CUDA 代码时，经常会出现下面的 lint 错误：

![](./figs/2025-11-10_22-33.png)

这是一个非常奇怪的错误。如此简单的插入运算符 `<<` 怎么会导致报错呢？

虽然代码依旧可以正确编译和运行，但这个 lint 错误实在太烦人了。

我在网上查了很久都没有找到解决方案。最终，我找到了问题的根源。

CUDA 工具包升级到 13 版本后，NVIDIA 悄悄地从 `targets/x86_64-linux/include` 中移除了旧的头文件 `texture_fetch_functions.h`。然而，Clang 仍然在其 CUDA 运行时包装器（`/usr/lib/clang/21/include/__clang_cuda_runtime_wrapper.h`）中无条件地在每个 CUDA 编译单元末尾执行 `#include "texture_fetch_functions.h"`。当 clangd 重放编译命令进行语义分析时，这个包装器会尝试包含该头文件，结果找不到，于是在处理 `<iostream>` 之前就终止了解析。由于解析过早停止，内置的 CUDA 关键字（例如 `__global__`）和标准库别名（如 `std::ostream`）都没有被注册，这就是 clangd 最终声称 `std::cout` 是 `int` 且 `<<` 无效的原因。

不过，这个问题其实有一个非常简单的解决办法：手动补上缺失的头文件。只要在某个包含路径中添加一个空的 `texture_fetch_functions.h`，lint 错误就会消失。

更多细节可以查看仓库中的 `.clangd` 文件以及位于 `./include_oldcuda` 下的那个空头文件。
