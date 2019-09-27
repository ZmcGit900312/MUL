# Current 类的说明文档

## 功能

  Current类主要用于记录求解后所得的电流，它只能记录MoM，AIM和Array其中一种格式，但是可以记录不同*frequency*的电流系数用于计算远场辐射。

## 文件格式

该二进制文件主要由**信息区**和**数据区**组成

### 信息区

1.  `enum category`表示类型：MoM=0，AIM=1，Array=2

2.  `int _numberOfConfig`表示电流共有几组，负数保留
### 数据区

`category=0,1` 的模式，配置$[1,numberofConfig]$:

-   `double _frequency` 频率
-   `size_t _unknowns` 未知数个数
-   `dcomplex _data[unknowns]` 电流系数

`category=2` 对应于阵列模式，配置$[1,numberofConfig]$:

-   `double _frequency` 频率

-   `size_t _numberOfElements` 单元个数

-   `size_t _elementUnknowns` 单元未知数

-   `size_t _unknowns` 整体未知数

    单元区，单元个数$[1,numberOfElements]$:

    -   `size_t row` 单元行
    -   `size_t col` 单元列
    -   `dcomplex _data[elementUnknowns]` 每个单元未知数个数

## 相关类与函数

### UML类图

在**Current.h**中的UML类图如下

![Current](.\res\Current.jpg)

#### `CurrentInfo`

`CurrentInfo`为单例，主要保留信息区内容

相关函数的作用如下

-   `ReadCurrent(const char* filename)` 用于从文件读取电流
-   `SaveCurrent(const char* filename)`用于向文件写入电流
-   `Reformat(EImpedance ty=MoM)`用于格式化`CurrentInfo`为指定格式

成员变量：

-   `_numberOfConfig`用于记录有几组结果
-   `category`为格式，其中MoM和AIM是相同的
-   `Current`是记录具体数据的链表

#### `ElementCurrent`

`ElementCurrent`用于记录MoM以及AIM格式的电流信息

相关函数的作用如下

-   `ReadBinary(ifstream &ifs)` 用于从输入流读取二进制数据
-   `SaveBinary(ofstream & ofs)`用于向输出流写入二进制数据
-   `GetCurrent(size_t id)`用于获取指定位置的电流
-   `EMCParameterUpdate()`用于更新切换配置时，全局变量*frequency*以及相关参数的更新，否则计算远场和填充矩阵都会有错误

成员变量：

-   `Tag`表示该配置的文件名
-   `_unknowns`表示未知数个数
-   `_frequency`频率
-   `_data`存储电流系数的向量

#### `ArrayCurrent`

`ArrayCurrent`继承自`ElementCurrent`,它用于记录Array格式的电流信息

相关函数的作用如下

-   `ReadBinary(ifstream &ifs)` 用于从输入流读取二进制数据
-   `SaveBinary(ofstream & ofs)`用于向输出流写入二进制数据
-   `dcomplex GetCurrent(size_t id, int x, int y)`重写了`ElementCurrent`的函数，用于获取带有`x`和`y`阵列偏移的电流基函数，这里的`id`仅仅是单元的索引

成员变量：

-   `Tag`表示该配置的文件名
-   `_unknowns`继承父类，表示总的未知数个数
-   `_elementUnknowns`为单元未知数个数
-   `_numberOfElement`表示总的单元个数
-   `_arrayBiasX`和`_arrayBiasY`分别表示阵列两个方法的偏移距离
-   `_array`是一个链表，用于记录每个单元的具体位置，从而可以支持稀疏矩阵的计算
-   `_frequency`频率
-   `_data`存储电流系数的向量

