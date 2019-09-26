# Current 二进制文件存储格式

## 文件格式

该二进制文件主要由**信息区**和**数据区**组成

### 信息区

1.  `enum category`表示类型：其中0为MoM，1表示AIM，2表示Array，负数保留

2.  `int _numberOfConfig`表示电流共有几组，负数保留
### 数据区

`category=0,1` 的模式，配置$[1,numberofConfig]$:

-   `double frequency` 频率
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

#### `CurrentInfo`

`CurrentInfo`为单例，主要保留信息区内容

-   `ReadCurrent(const char* filename)` 用于从文件读取电流
-   `SaveCurrent(const char* filename)`用于向文件写入电流
-   `Reformat(EImpedance ty=MoM)`用于格式化`CurrentInfo`为指定格式 

```C++
class CurrentInfo
		{
		public:
			static CurrentInfo* GetInstance() { return &_instance; }
			~CurrentInfo() { this->Reformat(); }

			int _numberOfConfig=0;
			EImpedance category=MoM;
			vector<ElementCurrent*> Current;

			bool ReadCurrent(const char* fileName);
			bool SaveCurrent(const char* fileName);
			void Reformat(EImpedance ty=MoM);
		private:
			CurrentInfo(){ this->Reformat(); }
			CurrentInfo operator=(CurrentInfo&) = delete;
			CurrentInfo(CurrentInfo&) = delete;

			static CurrentInfo _instance;
		};
```

