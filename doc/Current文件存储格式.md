# Current 二进制文件存储格式

## 文件格式

该二进制文件主要由**信息区**和**数据区**组成

### 信息区

1.  `int type`表示类型：其中0为常规，1表示阵列，负数保留

2.  `int numberOfConfig`表示电流共有几组，负数保留
### 数据区

`type=0` 的模式，配置$[1,numberofConfig]$:

-   `double frequency` 频率
-   `size_t unknowns` 未知数个数
-   `dcomplex currents[unknowns]` 电流系数

`type=1` 对应于阵列模式，配置$[1,numberofConfig]$:

-   `double frequency` 频率

-   `size_t numberOfElements` 单元个数

-   `size_t elementUnknowns` 单元未知数

-   `size_t wholeUnknowns` 整体未知数

    单元区，单元个数$[1,numberOfElements]$:

    -   `size_t row` 单元行
    -   `size_t col` 单元列
    -   `dcomplex currents[elementUnknowns]` 每个单元未知数个数

## 相关函数

