# developing `0.0.0`

- `26.5.5`
    - 解析性能大幅优化
    - 修复了dump的格式化bug
    - json库基本可用

- `26.5.4`
    - json解析状态机逻辑完工 (unicode转义(`'\n'`)的解析和字符串转数字待实现, 预留接口)
    - 调试状态机, 一些bug, 段错误一整天
    - 可正常解析json

- `26.5.3`
    - 开始实现json解析状态机

- `26.5.1~2`
    - json dump功能
    - 核心的json对象本身功能正常

- `26.4.30`
    - 微调, 类型转换等
    - reinterpret_cast被简化成tools::memcast/mut_memcast
    - text logo

- `26.4.28`
    - 由于依赖大量STL头文件, 编译速度过于感人, 为提高开发效率使用了预编译模块
    - 添加./compile.sh
    - type_tools.cpp更名为tools.cpp

- `26.4.27`
    - Unicode.cpp项目完善并支持模块

- `26.4.26`
    - 确定泛型框架与内存模型
    - json类的泛性及递归列表初始化可正常使用, 解决无限递归问题
    - 创建了tools.cpp文件(命名type_tools.cpp)
    - 记得是在这时候开始使用clang-format

- `26.4.25`
    - 开了Unicode.cpp库项目(其实json用不太到)
    - 创建了项目文件夹

- `26.4.24`
    - 读nlohmann/json源码
    - 最初构想