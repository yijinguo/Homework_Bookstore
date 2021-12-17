## **书店管理系统——BOOKSTORE**

文档作者：郭一锦

partner：胡珂雅

2021-12-17

版本信息 ：开发者自主修改第一次

### 一、程序功能概述

实现一个书店管理系统，由账户系统、图书系统、日志系统构成，服务于店主（超级管理员）、员工（管理员）以及顾客（用户），向其提供账户管理、图书信息查询、交易财务记录、日志信息查询等书店基本功能。



### 二、主体逻辑说明

该系统的运行方式是执行输入的合法指令，故而需要对输入指令进行读取、异常处理和正确执行的操作，通过派生类的方式实现不同权限账户的不同功能。考虑到书店数据的庞杂，该系统使用文件读写的方式保存各系统的主体数据，故而需要基于文件存储实现一个 *Key-value database（键值数据库）*。



### 三、代码文件结构

(1)main.cpp   读入输入指令

(2)parser.h   解析指令

(3)statement.h   执行指令；包含五个类

(4)database.h   实现数据存储

(5)error.h   异常处理

注意，对于各h头文件，请自行建立对应的cpp实现文件。



### 四、各代码文件的具体说明

###### 1.parser.h : 解析指令，将指令处理成Commmand型（一个自定义的类）变量

```c++
//解构输入的命令行
Command parseCommand(std::string commandLine);

//根据字符串判断指令类型，Usertype为一个自定义的枚举类型，详细说明见下
Usertype getType(std::string command);
```



###### 2.statement.h : 对三个系统所有指令的具体实现，根据权限不同，设一个接口类和四个派生类。

本书店包括三个系统，对于账户和图书系统中每位用户和每本图书的信息，请用两个结构体封装。

```c++
/*用户信息*/
struct UserInf {
    char user_ID[30];
    char password[30];
    char user_name[30];
    int Priority;
};

/*图书信息*/
struct BookInf {
    char ISBN[20] = "";
    char book_name[60] = "";
    char author[60] = "";
    char keyword[60] = "";
    int quantity = 0;
    float price = 0;
    float total_coat = 0;
};
```

本书店系统的所有账户的操作均需保持登录状态，对此，请使用如下的登录栈实现：

```c++
std::stack<UserInf> accountIn;
```

本书店系统中的账户权限不同，对此，请自定义一个枚举类型，如下：

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
    /*对于该函数的实现有文件输出格式要求，详见四、3中关于文件的说明*/
    void Log();

    virtual void execute();

};
```



###### 3.database.h  ：实现数据存储，管理文件中的数据

利用本代码文件中的函数，实现一个 *Key-value database（键值数据库）*

以下对本代码文件中简要说明。

database类：

对于具体实现方式，本开发文档不做限制，推荐使用块状链表。

以下给出一个实现块状链表的模板类，仅供参考。

```c++
/*该模板类的目的为实现一个类似于std::set的文件存储结构*/

#define MAXSIZE num /*your-ideal-num*/

//Data表示存储的信息类型
template<class Data>
class database {

private:

    //块链中的信息按照所存储条目中具有唯一性的信息排序
    struct Node {
        int num;
        int next;
        Data array[MAXSIZE];
    };
    int head = -1;//用于记录头节点在文件中的位置
    int sizeData = sizeof(Data);
    int sizeNod = sizeof(Node);
    std::fstream dataFile;
    std::string fileName = "your-ideal-name";
    //节点分割
    void spilt(int nodLoc);
    //节点合并
    void merge(int nodLoc);
    //根据索引直接找到文件读写位置并返回;找不到则返回-1
    int findLoc(std::string index);
    
public:

    /*以下函数允许自行更改参数，完成功能后告知开发者即可*/
    
    //插入数据;策略：扫过链表，找到相应的位置，分割节点并插入
    void insert(std::string index, std::string data);
    //删除数据;策略：扫过链表，找到相应的位置，分割节点并删除
    void erase(std::string index, std::string data);
    //修改信息
    /*请自行实现函数定义*/
    
    /*如有需要可自行补充函数*/

};
```

（申明：由于开发者水平有限，该部分执行者具有较大自主权，如有必要可以无视database.h内所有给定的类，自行实现，告知开发者即可）



###### 4.error.h :  异常处理，捕获输入指令时出现的异常并处理。

包含一个异常类MyError，开发者可以自主选择构造函数和成员函数，完成异常处理。

```c++
class MyError : public std::exception {
    /*该部分开发者具有较大自主权
     *根据异常情况自行决定构造函数和成员函数*/
public:
    MyError();
    /*Your code*/
};
```



### 五、数据文件说明

在此对本书店系统涉及的数据文件进行说明：

一）程序运行中用于储存数据的文件：

(1) 账户系统 ：需要一个文件储存所有的账户信息，以结构体UserInf为单位，其中[User-ID]唯一。

(2) 图书系统 ：需要一个文件储存所有的图书信息，以结构体BookInf为单位，其中[ISBN]唯一。

二）根据指令生成的输出文件：

这部分文件随指令的进行发生更改。

(1)需要一个文件保存所有财务交易信息；

需要在购买图书和图书进货指令时写入信息，信息格式：

购买图书  `buy : [User-ID] buy [ISBN] [Book-Name] [Quantity] for [total-cost]  `

图书进货 `import : [User-ID] import [ISBN] [Quantity] [Total-Cost]`

(2)需要一个文件保存整个系统操作类的信息，信息格式：`[User-ID] + /每行指令的输入信息/`

注意，`每行指令的输入信息`可以根据每条指令的不同自主选择可读性更强的保存格式，由执行者自主决定，无需向开发者说明。

(3)需要多个文件分别保存每个员工的操作记录，建议以各员工ID命名,信息格式：

每位员工的个人文件中，每条信息的格式：`/每行指令的输入信息/ `

注意，`每行指令的输入信息`可以根据每条指令的不同自主选择可读性更强的保存格式，由执行者自主决定，无需向开发者说明。

(4)需要一个文件保存日志指令，该文件在执行log指令时才进行更改，输出内容为(2)(1)两个文件中信息的合并。



### 六、其他补充说明

1.本文档开发过程中对任何bonus内容均未予考虑。

2.本文档中的文件名、函数名、变量名等任何命名，在保证可读性的前提下可以根据个人偏好自行修改，不需要通知开发者。

3.实现过程中，在合理范围内可以自行增加全局变量、全局函数、类的私有成员等，实现功能后与开发者统一说明即可。

4.（此条database.h文件不适用）如果想要更改代码文件的个数或结构、减少类的私有成员或者修改给出的函数的参数等，请及时与开发者协商，切勿擅自修改。

2.执行方若有任何疑问或质疑可随时提出。