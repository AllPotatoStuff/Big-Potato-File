# Building and running the project (CMake)

This project now uses **CMake** instead of Make directly. The idea: CMake generates build files (here, **Visual Studio** project files), and `cmake --build` compiles them — no matter what tool is underneath.

## Building from the terminal

```bash
# Configure (generates the build/ folder with the Visual Studio projects)
cmake -B build -S .

# Build
cmake --build build

# Build in parallel (faster)
cmake --build build --parallel 8

# Clean
cmake --build build --target clean

# Full reset (wipes the CMake cache, useful if the project state gets corrupted)
rmdir /s /q build
cmake -B build -S .
```

## Building from VS Code

The tasks have been adapted in `.vscode/tasks.json`:

| Task                     | Equivalent to the old `make`                   |
| ------------------------ | ---------------------------------------------- |
| `CMake Build`            | `make depend; make`                            |
| `CMake Build Parallel`   | `make depend; make -j8`                        |
| `CMake Rebuild`          | `make depend; make clean; make`                |
| `CMake Rebuild Parallel` | `make depend; make clean; make -j8`            |
| `CMake Clean`            | `make clean`                                   |
| `CMake Full Reset`       | deletes `build/` and reconfigures from scratch |

Run them with **Ctrl+Shift+B** (default build) or **Ctrl+Shift+P → Tasks: Run Task**.

## Debugging from VS Code

The configurations in `.vscode/launch.json` (F5) run the matching build task, then attach the debugger:

- **Build and Run**
- **Build Parallel and Run**
- **Re-Build and Run**
- **Re-Build Parallel and Run**
- **Clean**

⚠️ **Important change**: the debugger switched from `cppdbg` (gdb / MSYS2) to **`cppvsdbg`** (Visual Studio debugger), meaning the code is compiled with `cl.exe` (MSVC), not `g++`. If you'd rather use MSYS2/g++, force the Ninja or MinGW Makefiles generator at configure time instead:

```bash
cmake -B build -S . -G "Ninja"
```

In that case, keep `cppdbg` + gdb in `launch.json`, and the executable will land directly in `build/` (not `build/Debug/`).

## Executable path

With the **Visual Studio** generator (multi-config), the executable ends up in:
```
build/Debug/bigpotatofile.exe    (Debug mode, default)
build/Release/bigpotatofile.exe  (Release mode, with --config Release)
```

To build in Release:
```bash
cmake --build build --config Release
```

## Passing arguments to the tool
 
`bigpotatofile.exe` takes a `-t/--tool` argument plus tool-specific options (e.g. `-p/--path` for `scan` and `index`):
 
```
bigpotatofile.exe -t scan -p <folder>
bigpotatofile.exe -t index -p <folder>
```
 
**From the terminal**: just pass them after the exe, as shown above.
 
**From VS Code (F5)**: arguments go in the `"args"` array of the matching entry in `.vscode/launch.json`. Two ready-to-use configurations are included:
 
- **Run: scan** — runs `-t scan -p ${workspaceFolder}`
- **Run: index** — runs `-t index -p ${workspaceFolder}`
`${workspaceFolder}` is just a placeholder pointing at the project root — replace it with whatever target folder you actually want to scan/index, e.g.:
 
```jsonc
"args": ["-t", "scan", "-p", "C:/some/other/folder"]
```
 
Pick one of these from the Run and Debug dropdown in VS Code instead of "Build and Run" when you want a specific tool run with debugging attached.

## Quick troubleshooting

If the build fails with strange errors after editing `CMakeLists.txt`, try the **CMake Full Reset** task first — most odd issues come from a stale CMake cache in `build/`.