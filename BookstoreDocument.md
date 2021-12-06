## **书店管理系统——BOOKSTORE**

文档作者：郭一锦

partner：胡珂雅

2021-12-5

### 一、程序功能概述

实现一个书店管理系统，由账户系统、图书系统、日志系统构成，服务于店主（超级管理员）、员工（管理员）以及顾客（用户），向其提供账户管理、图书信息查询、交易财务记录、日志信息查询等书店基本功能。



### 二、主体逻辑说明

该系统的运行方式是执行输入的合法指令，故而需要对输入指令进行读取、异常处理和正确执行的操作，通过派生类的方式实现不同权限账户的不同功能。考虑到书店数据的庞杂，该系统使用文件读写的方式保存各系统的主体数据，故而需要基于文件存储实现一个 *Key-value database（键值数据库）*。



### 三、代码文件结构

(1)main.cpp   读入输入指令

(2)parser.hpp   解析指令

(3)statement.hpp   执行指令；包含五个类

(4)database.hpp   实现数据存储

(5)error.hpp   异常处理

注意，对于各hpp头文件，请自行建立对应的cpp实现文件。



### 四、各代码文件的具体说明

###### 1.parser.hpp : 解析指令，将指令处理成Commmand型（一个自定义的类）指针

```c++
//解构输入的命令行
Command *parseCommand(std::string commandLine);

//根据字符串判断指令类型
Usertype getType(std::string command);
```



###### 2.statement.hpp : 对三个系统所有指令的具体实现，根据权限不同，设一个接口类和四个派生类。

本书店系统的所有账户的操作均需保持登录状态，对此，请使用如下的登录栈实现：

```c++
std::stack<std::string> your_ideal_name;//命名自选
```

本书点系统中的账户权限不同，对此，请自定义一个枚举类型，如下：

```c++
enum Usertype {visitor, customer, staff, shopkeeper};
```

以下对本代码文件的结构进行具体说明。

每个权限的类都包含一个execute函数执行输入的指令，其余函数用于根据不同的指令名称实现不同的功能。

每个相邻类之间都存在继承与派生关系。在本文档中，行注释开头标注$表示重定义基类中的同名函数，标注&表示直接继承基类中的同功能函数，标注*表示该函数实现的功能在OJ不会被测试正确性。

是否重载或直接继承取决于本题的要求，为避免开发者的疏漏带来错误，实现时请以题面为准；在保证正确实现的前提下，是否重载或直接继承由执行者决定，事后向开发者说明即可。

除接口类外，每个类中分别有command和commandLine两个字符串变量，分别表示执行的指令名称（即第一个单词）和除去该词后的指令行。当然，执行者也可以根据个人习惯将其修改为一个字符串类型的变量，表示整行指令，无需向开发者说明。

(1) Command类 : 接口类，使所有权限账户实现执行函数execute。

```c++
class Command {

public:

    Command();

    virtual ~Command();

    virtual void execute() = 0;

};
```

（2）Visitor类 ：权限为{0}的账户，即游客所能实现的指令。继承Command类。

```c++
class Visitor : public Command {

private:

    std::string command;

    std::string commandLine;

public:

    Visitor();

    ~Visitor();

    //登录账户su [User-ID] ([Password])?
    void Login();
    //注册账户register [User-ID] [Password] [User-Name]
    void Register();

    virtual void execute();

};
```

(3) Customer类 ： 权限为{1}的账户，即顾客所能实现的指令。继承Visitor类。

```c++
class Customer : public Visitor {

private:

    std::string command;
    std::string commandLine;

public:

    Customer();

    ~Customer();

    //$登录账户su [User-ID] ([Password])?
    void Login();
    //$注册账户register [User-ID] [Password] [User-Name]
    void Register();
    //注销账户logout
    void Logout();
    //修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    void Passwd();

    //检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    void Show();
    //购买图书buy [ISBN] [Quantity]
    void Buy();

    virtual void execute();

};
```

(4) Staff类 ： 权限为{3}的账户，即员工所能实现的指令。继承Customer类。

```c++
class Staff : public Customer {

private:

    std::string command;
    std::string commandLine;

public:

    Staff();

    ~Staff();

    //$登录账户su [User-ID] ([Password])?
    void Login();
    //$注册账户register [User-ID] [Password] [User-Name]
    void Register();
    //&注销账户logout
    //&修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    //创建账户useradd [User-ID] [Password] [Priority] [User-Name]
    void Useradd();

    //&检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    //$购买图书buy [ISBN] [Quantity]
    void Buy();

    //选择图书select [ISBN]
    void Select();
    //修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    void Modify();
    //图书进货import [Quantity] [Total-Cost]
    void Import();

    //*生成员工操作记录report myself
    void Report();

    virtual void execute();

};
```

(5) Shopkeeper类 ： 权限为{7}的账户，即店主所能实现的指令。继承Staff类。

```c++
class Shopkeeper : public Staff {

private:

    std::string command;
    std::string commandLine;

public:

    Shopkeeper();

    ~Shopkeeper();

    //$登录账户su [User-ID] ([Password])?
    void Login();
    //**注册账户 : 店主账户初始化，不进行注册
    //&注销账户logout
    //$修改密码passwd [User-ID] ([Old-Password])? [New-Password]
    void Passwd();
    //$创建账户useradd [User-ID] [Password] [Priority] [User-Name]
    void Useradd();
    //删除账户delete [User-ID]
    void Delete();

    //&检索图书show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    //&购买图书buy [ISBN] [Quantity]
    //&选择图书select [ISBN]
    //&修改图书modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    //&图书进货import [Quantity] [Total-Cost]

    //*&生成员工操作记录report myself
    //财务记录查询show finance ([Time])?
    void showFinance();
    //*生成财务记录报告report finance
    void reportFinance();
    //*生成全体员工工作情况报告report employee
    void reportEmployee();
    //*生成日志log
    void Log();

    virtual void execute();

};
```



###### 3.database.hpp  ：实现数据存储，管理文件中的数据

利用本代码文件中的函数，实现一个 *Key-value database（键值数据库）*

在此对本书店系统涉及的数据文件进行申明：

(1) 账户系统 ：需要一个文件储存所有的账户信息，其中[User-ID]唯一。

(2) 图书系统 ：需要一个文件储存所有的图书信息，其中[ISBN]唯一。

(3) 日志系统 ：

需要一个文件储存所有财务交易信息；

需要一个文件储存整个系统的全部操作记录；

需要多个文件分别储存每个员工的操作记录，建议以各员工ID命名；

以下对本代码文件中简要说明。

```c++
template<class T>
class database{
private:
    std::fstream file;
    std::string file_name;
    int sizeofT = sizeof(T);
    int position;//定位
    
    //可以自行添加私有成员变量
    
public:
    database() = default;
    ~database();
    database(const std::string& file_name) : file_name(file_name){}
    //以[index]为索引，插入数据data
    void Insert(std::string index, T data);
    //以[index]为索引，删去数据
    void Delete (std::string index);
    //以[index]为索引，获取数据
    T Find (std::string index);
    
    //可以根据需要自行添加成员函数

};
```

另外，对于具体实现方式，本开发文档不做限制，推荐使用块状链表。

以下给出一个实现块状链表的模板类，仅供参考。

```c++
template<class T>
    
class BlockList {

private:

    int num = 0;//记录读写文件位置
    struct list{
        //该容器可以允许自行替换
        std::map<T, int> position;
        list *next;
        list *front;
    };
    list *head;
    list *rear;

public:

    /*以下函数允许自行更改参数，完成功能后告知开发者即可*/

    //根据索引直接找到文件读写位置并返回
    int find(T index);
    //插入数据
    //策略：扫过链表，找到相应的位置，分割节点并插入
    void insert(T index, std::string data);
    //删除数据
    //策略：扫过链表，找到相应的位置，分割节点并删除
    void erase(T index, std::string data);
    /*节点分割与合并两个函数,自行填充参数；如果认为多余可以忽略*/
    //节点分割
    void spilt();
    //节点合并
    void merge();

    /*如有需要可自行补充函数*/

};
```

（申明：由于开发者水平有限，该部分执行者具有较大自主权，如有必要可以无视database.hpp内所有给定的类，自行实现，告知开发者即可）

###### 4.error.hpp :  异常处理，捕获输入指令时出现的异常并处理。

请自行进行异常处理，实现对各种异常情况的正确处理即可。



### 五、其他补充说明

1.本文档开发过程中对任何bonus内容均未予考虑。

2.本文档中的文件名、函数名、变量名等任何命名，在保证可读性的前提下可以根据个人偏好自行修改，不需要通知开发者。

3.实现过程中，在合理范围内可以自行增加全局变量、全局函数、类的私有成员等，实现功能后与开发者统一说明即可。

4.（此条database.hpp文件不适用）如果想要更改代码文件的个数或结构、减少类的私有成员或者修改给出的函数的参数等，请及时与开发者协商，切勿擅自修改。

2.执行方若有任何疑问或质疑可随时提出。