# MonoEnableDebugger
- 用MonoEnableDebugger来加载需要调试的exe
    ```bash
    ./MonoEnableDebugger.exe Target.exe
    ```
- 使用`dnSpy` 进行调试（可能需要将最后一个参数改成[`suspend=n`](https://github.com/cokkeijigen/MonoEnableDebugger/blob/6ac13d7e580c0b564e8c06f2e2b4a2824ef3489d/MonoEnableDebugger/src/utils/mono_utils.hpp#L18)或[`defer=y`](https://github.com/cokkeijigen/MonoEnableDebugger/blob/4b3b5b08a2059af458b727c0c79743bb05363a67/MonoEnableDebugger/src/utils/mono_utils.hpp#L26)）<br>
    ![Image text](https://raw.githubusercontent.com/cokkeijigen/MonoEnableDebugger/master/Pictures/dnspy_dbg.png)

- 使用`Microsoft Visual Studio`进行调试<br>
    ![Image text](https://raw.githubusercontent.com/cokkeijigen/MonoEnableDebugger/master/Pictures/vs_dbg.png)
