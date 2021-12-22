#ifndef UNTITLED3_ERROR_H
#define UNTITLED3_ERROR_H

#include <iostream>
#include <exception>

/*处理整行空白字符*/
class BlankLineException: public std::exception{};

/*quit和exit退出系统*/
class OutException : public std::exception{};

/*指令非法或操作失败，输出Invalid*/
class BasicException : public std::exception{};

#endif //UNTITLED3_ERROR_H
