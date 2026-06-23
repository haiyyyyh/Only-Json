# developing `0.8.0`

## day `26.6.24`
- SAX parser添加NaN和Inf支持

## day `26.6.7`
- 添加README,小改,上传github

# developing `0.7.0`

## day `26.6.2`
- 将DOM json的核心逻辑和一些包装\别名\工具分离,避免引入std::string,std::map等的头文件,核心逻辑在header/base.hpp
- SAX解析器终于支持注释

# developing `0.6.0`

## day `26.6.1`
- 让DOM json最低能支持到C++17版本
- 此后为确保兼容性,打算将.clangd配置为C++17

## day `26.5.31`
- 基本实现开放寻址哈希表(不在此项目文件夹中)

# developing `0.5.1`

## day `26.5.30`
- DOM的解析状态机分支添加了分支提示likely/unlikely,可惜性能提升不明显
- 打算自己实现开放寻址哈希表

# developing `0.5.0`

## day `26.5.29`
- SAX parser修复了空数组和空对象会报错的bug
- SAX的parser模板允许传入flag,可以指定一些非标准的解析行为比如允许尾随逗号,不处理转义,将数字以字符串读入等等
- DOM json更改了一些源码的排版

# developing `0.4.2`

## day `26.5.26`
- 更改basic_json在const下的API,新增了一些函数的const重载

# developing `0.4.1`

## day `26.5.25`
- 无更改,仅clang-format格式

# developing `0.4.0`

## day `26.5.23`
- basic_json和一些内部API小改
- 模板参数变动,basic_json允许自己提供string,array,object和allocator容器实现
- json(basic_json别名,用STL基础容器) 解析19MB*10 耗时 1.292s 左右
- json_with_pool使用hai::pool内存池,耗时 1.026s 左右

# developing `0.3.1~2 DEV`

## day `26.5.20~22`
- 修复了SAX解析引擎的一个bug,还得到了大幅度性能提升
- SAX空回调解析性能: 19MB*10 耗时 171.6~184.8ms
- SAX提供了较少检查的可选项,在保证json数据正确的情况下,使用`parse_no_check()`可提升至 150.4~157.6ms
- 使用DOM的only-json开发项目
- 修复了一个崩溃bug
- 项目结构小改

###day  Crash bug #004 (已解决)
> SAX解析器的查表性能优化,字符char 转 unsigned long long产生超级大数越界,被ASAN拦截,libc下UB但未报错,并且由于整个json文件没有空格,bug未暴露

## day `26.5.18~19`
- 重构了整个状态机 (原本是直接从DOM json里移植过来的)
- 使用goto跳转代替 递归下降+大循环+多状态标志+分支密集混杂 的状态机
- SAX 解析器核心空转性能: 19MB*10 耗时轻松达到226.5ms
- 后续打算在热点小小的搞个simd (AVX/AVX2批处理优化) 起飞

# developing `0.3.0 DEV`

## day `26.5.17`
- SAX 解析器空壳(消费者handler无操作)空转性能: 19MB json数据解析10次 耗时296.8ms
- 状态机指令和分支偏多,可能需进一步优化 (为了赶上yyjson)

## day `26.5.16`
- 大型重构,改用SAX架构

# developing `0.2.3`

## day `26.5.15`
- 发现性能不如yyjson和rapidjson,自此开始卷性能
- 尝试优化性能,小改

# developing `0.1.3`~`0.2.2`

## day `26.5.13~14`
- API组织大面积重构
- 不再使用单一的json,而是template basic_json<>,允许自定义分配器,json为别名
    - 默认使用std::allocater
- 逻辑小更改,行为不变
- 修复了dump和fast_dump的bug
- 新增了比较运算符,并修复了一些类型问题

# developing `0.1.1~2`

## day `26.5.11~12`
- only-json第一次投入使用
- 更改了开发流程
    - 不再将开发的src.cpp和发布的json.hpp分离
    - 将json.cpp用于开发,完毕后写回json.hpp (in ./header/)
- 源码未大改,修复了一个dump时字符串输出可能出现的小bug
    - 原因只是switch case标签未及时跳出

# developing `0.1.0`

## day `26.5.10`
- 更改了一些解析文件\字符串的json初始化方式
- 小改源码结构
- 新增了在错误字符串中查看错误位置的函数`check_failed_part`
- 重构数字状态机
- 完整支持json标准数字字面量
- 存在一定的不稳定性
    - 字面量太大会unsigned long long溢出环绕
    - 精度有限,无原生高精度算法

###day  性能大约是nlohmann/json的4倍
> 编译参数 `-O2 -g3 -fno-omit-frame-pointer -fno-exceptions -fno-rtti`
```json
[3.1415926,-3.1415926,1234.5678,1234.003e+3,-9576.0042817e-3,2.11111111111111111111e10,18446744073709551615,......]
```
跑for循环解析多次
- only-json: 3.19s
- nlohmann/json: 12.96s

###day  json库功能全部可用
- 字符串也完整支持了unicode UTF-16转义序列

## day `26.5.8`
- 小改状态机,新增数字字面量前导+号和e的大写体E的支持
- 更改项目结构

# developing `0.0.3~4`

## day `26.5.7`
> 期中考试前
- 支持以模块导入和单头文件包含
- 支持注释

# developing `0.0.2`

## day `26.5.6`
- 小改结构和API

# developing `0.0.1`

## day `26.5.5`
- 解析性能大幅优化
- 修复了dump的格式化bug
- json库基本可用,初始化了git,第一个版本定为v0.0.1

###day  Crash bug #003 (已解决)
> 诡异的崩溃,Asan常提示堆的 *越界写入*,还有 *内存泄漏* 和 *BUS*
- 详细:
    - 使用测试阶段的自定义容器"train",发现如果触发拷贝构造,会在某些地方离奇崩溃,还有跑到不该跑的分支
    - 在小json数据下无崩溃,中型json数据也不会,但数据完全同结构的大型json数据下会崩溃,且在外部无法复现,难以调试
- 原因:
    - 拷贝构造函数bug,length<=other.length更正为小于符号
    - clear函数bug,边界计算错误,length>=0改为不等于

## day `26.5.4`
- json解析状态机逻辑完工 (unicode转义(`'\n'`)的解析和字符串转数字待实现,预留接口)
- 调试状态机
- 可正常解析json

###day  Crash bug #002 (已解决)
> 离奇的段错误,非法指针与释放后使用
- 详细:
    - 当json数据含有string(且位于第一项),并且使用libstdc++时,并且出发SSO小串优化时会发生崩溃,Asan显示 *释放后使用* 等问题,还会有 *越界* 和 *内存泄漏*
    - 可较稳定复现
- 原因: 构造函数不当的memcpy,导致使用SSO的string容器栈指针失效,而长字符串堆分配\LLVM libc++策略不同,所以没有出现崩溃

## day `26.5.3`
- 开始实现json解析状态机

## day `26.5.1~2`
- json dump功能
- 核心的json对象本身功能正常

## day `26.4.30`
- 微调,类型转换等
- reinterpret_cast被简化成tools::memcast/mut_memcast
- text logo

## day `26.4.28`
- 由于依赖很多STL头文件,编译速度过于感人,为提高开发效率使用了预编译模块
- 添加./compile.sh
- type_tools.cpp更名为tools.cpp

## day `26.4.27`
- Unicode.cpp项目完善并支持模块

## day `26.4.26`
- tools.cpp文件被创建(命名type_tools.cpp)
- 确定泛型框架与内存模型
- 开始使用clang-format
- json类的泛性及递归列表初始化可正常使用

###day  Crash bug #001 (已解决)
> 数组和字典的列表初始化会产生段错误
- 原因: 列表初始化歧义引起的无限递归

## day `26.4.25`
- Unicode.cpp库项目创建,后发现json用不到
- 创建了项目文件夹

## day `26.4.24`
- 读nlohmann/json源码
- 最初构想