#ifndef UNTITLED3_ERROR_H
#define UNTITLED3_ERROR_H

#include <iostream>
#include <exception>

/*处理整行空白字符*/
class BlankLineException{};

/*quit和exit退出系统*/
class OutException{};

/*指令非法或操作失败，输出Invalid*/
class BasicException{};

/*在图书系统中选中图书，没有符合条件的图书，则创建仅有ISBN信息的图书*/
class CreateException{};

/*利用异常处理的特点进行一些处理*/
class DealException {};

#endif //UNTITLED3_ERROR_H
