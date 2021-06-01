#include "common/s4json_util.h"

#include <stack>


namespace S4{


bool searchJson(std::string str, int & start, int & stop, int* ec)
{
    stop = start = (int)str.find('{');
    if(start > (int)str.size()){
        if(ec){
            *ec = JSON_NO_START;
        }
        return false;  
    }

    std::stack<char> jsonstr;
    const char *p = str.c_str() + start;
    char startChar = *p;
    char endChar = '\0';
    bool isObject = false;//防止 {}{}的判断
    bool isArray = false;//防止[][]的判断
    bool json_end = false;

    while (*p != '\0')
    {
        endChar = *p;
        switch (*p)
        {
        case '{':
            if (!isObject)
            {
                isObject = true;
            }
            else  if (jsonstr.empty())//对象重复入栈
            {
                stop--;
                json_end = true;
                break;
                // return false;
            }
            jsonstr.push('{');
            break;
        case '"':
            if (jsonstr.empty() || jsonstr.top() != '"')
            {
                jsonstr.push(*p);
            }
            else
            {
                jsonstr.pop();
            }
            break;
        case '[':
            if (!isArray)
            {
                isArray = true;
            }
            else  if (jsonstr.empty())//数组重复入栈
            {
                if(ec){
                    *ec = JSON_ILLEGAL_STOP;
                }
                return false;  
            }
            jsonstr.push('[');
            break;
        case ']':
            if (jsonstr.empty() || jsonstr.top() != '[')
            {
                if(ec){
                    *ec = JSON_ILLEGAL_STOP;
                }
                return false;  
            }
            else
            {
                jsonstr.pop();
                if(jsonstr.empty()){
                    json_end = true;
                }
            }
            break;
        case '}':
            if (jsonstr.empty() || jsonstr.top() != '{')
            {
                if(ec){
                    *ec = JSON_ILLEGAL_STOP;
                }
                return false;  
            }
            else
            {
                jsonstr.pop();
                if(jsonstr.empty()){
                    // cout << "size=" << str.size()
                    //     << " start=" << start
                    //     << " stop=" << stop 
                    //     << " " << *p 
                    //     << "  " << str[stop] 
                    //     << ", json=" << str.substr(start, stop - start + 1) 
                    //     << endl;
                    json_end = true;
                }
            }
            break;
        case '\\'://被转义的字符,跳过
            p++;
            stop++;
            break;
        default:
            ;
        }
        
        if(json_end)
            break;

        stop++;
        p++;
    }

    if (jsonstr.empty())
    {
        //确保是对象或者是数组,之外的都不算有效 
        switch (startChar)//确保首尾符号对应
        {
        case  '{':
        {
            if (endChar == '}')
            {
                return true;
            }
            if(ec){
                *ec = JSON_ILLEGAL_STOP;
            }
            return false;  
        }
        case '[':
        {
            if (endChar == ']')
            {
                // return JSON_ILLEGAL_STOP;
            }
            if(ec){
                *ec = JSON_ILLEGAL_STOP;
            }
            return false;  
        }
        default:
            // return JSON_ILLEGAL_STOP;
            if(ec){
                *ec = JSON_ILLEGAL_STOP;
            }
            return false;  
        }

        // return true;
    }
    // return JSON_WAITING_STOP;
    if(ec){
        *ec = JSON_WAITING_STOP;
    }
    return false;
}

}//namespace S4
