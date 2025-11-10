#pragma once

// Minimal stub so clangd can satisfy __clang_cuda_runtime_wrapper.h when the
// actual CUDA 13+ header was removed. The real implementations are never used
// in this project, so an empty placeholder is enough for linting.
