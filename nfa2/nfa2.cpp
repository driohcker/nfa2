#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Token 类型枚举
enum TokenType {//更改
    RESERVED_WORD = 0,//识别之外

    IDENTIFIER,//标识符

    CONSTANT,//常数

    //OPERATOR,//运算符4
    PLUS,//加
    SUB,//减
    MUL,//乘
    DIV,//除

    //SEPARATOR,//分隔符7
    LEFT_BRACKET,//左括号
    RIGHT_BRACKET,//右括号
    LEFT_SQUARE_BRACKET,//左方括号
    RIGHT_SQUARE_BRACKET,//右方括号
    LEFT_CURLY_BRACKET,//左花括号
    RIGHT_CURLY_BRACKET,//右花括号
    SEMICOLON,//分号
    COMMA,//逗号
    DOT,//点号

    //KEY_WORD,//关键字16
    MAIN,//main
    INT,//int
    STRING,//string
    PUBLIC,//public
    COUT,//cout
};

// Token 结构体，用于表示词法单元
struct Token {
    string lexeme;
    TokenType type;
};

bool ifChar(char c) {
    if (c >= 'a' && c <= 'z') return true;
    else return false;
}
//判断是否为数字
bool ifNumble(char c) {
    if (c >= '0' && c <= '9')return true;
    else return false;
}
//解释类型
string getType_Chinese(int code, int no) {
    if (code == 0)return "\033[31;1m无法识别的字符,错误在第" + to_string(no) + "行\033[0m";
    else if (code == 1)return "标识符";
    else if (code == 2)return "常数";
    else if (code < 7)return "运算符";
    else if (code < 16)return "分隔符";
    else return "关键字";
}
// 词法分析器类
class Lexer {
public:
    Lexer(string input) : input(input), position(0) {}

    // 从输入中获取下一个 token
    Token getNextToken() {
        while (position < input.size() && isspace(input[position])) {
            // 跳过空白字符
            position++;
        }
        
        if (position == input.size()) {
            // 已经到达输入末尾
            return Token{ "", RESERVED_WORD };
        }

        char currentChar = input[position];

        if (isSignword(currentChar)) {
            // 识别标识符
            return getIdentifier();
        }
        else if (isDigit(currentChar)) {//非自定义
            // 识别常数
            return getConstant();
        }
        else if (isOperator(currentChar)) {
            // 识别运算符
            return getOperator();
        }
        else if (isSeparator(currentChar)) {
            // 识别分隔符
            return getSeparator();
        }
        else {
            // 无法识别的字符，作为错误处理或者跳过
            position++;
            return Token{ "", RESERVED_WORD};
        }
    }
    void returnChar() {
        cout << "word:'" << input[position] << "'" << endl;
    }//调试函数，显示现在position所指的字符是什么
    int getPosition() {
        return position;
    }//返回position的值
private:
    string input;
    size_t position;//字符指针
    string operators = "+-*/";//操作符字符表
    string separators = "(){}[];,.";//分隔符字符表
    string keywords[5] = { "main","int","string","public","cout"};

    
    // 检查字符是否为运算符
    bool isOperator(char c) {
        return operators.find(c) != string::npos;
    }

    // 检查字符是否为分隔符
    bool isSeparator(char c) {
        return separators.find(c) != string::npos;
    }

    //检查是否为标识符
    bool isSignword(char c) {
        int n = 0;
        
        if (ifChar(c)) {
            position++;
            n++;
            
            while (ifChar(input[position]) || ifNumble(input[position]) || input[position] == '_') {
                position++;
                n++;
                
            }
            position=position-n;
            return true;
        }else return false;
    }
    
    //检查是否为常数
    bool isDigit(char c) {
        int n = 0;
        int dot = 1;
        if (ifNumble(c)) {
            position++;
            n++;

            while (ifNumble(input[position])) {
                if (input[position+1] == '.' && ifNumble(input[position + 2]) && dot == 1) {
                    dot--;
                    position++;
                    n++;
                }
                position++;
                n++;
            }
            position = position - n;
            return true;
        }else return false;
    }
    // 获取标识符后进行再判断是否为关键字
    Token getIdentifier() {
        string lexeme = "";
        int keyword_code = -1;
        while (position < input.size() && (isalnum(input[position]) || input[position] == '_')) {
            lexeme += input[position];
            position++;
        }
        
        for (int i = 0; i < 5; i++) {
            if (keywords[i] == lexeme) {
                keyword_code = i;
                break;
            }
        }
        //cout << keyword_code << endl;
        switch (keyword_code) {
            case 0:
                return Token{ lexeme , MAIN };
            case 1:
                return Token{ lexeme , INT };
            case 2:
                return Token{ lexeme , STRING };
            case 3:
                return Token{ lexeme , PUBLIC };
            case 4:
                return Token{ lexeme , COUT };
            default:
                break;
        }
        
        return Token{ lexeme, IDENTIFIER};
    }
    
    // 获取常数
    Token getConstant() {
        string lexeme = "";
        int dot = 1;
        while (position < input.size() && isdigit(input[position])) {
            if (input[position + 1] == '.' && ifNumble(input[position + 2]) && dot == 1) {
                dot--;
                position++;
                lexeme += input[position - 1];
            }
            lexeme += input[position];
            position++;
        }
        return Token{ lexeme, CONSTANT };
    }

    // 获取运算符
    Token getOperator() {
        char lexeme = input[position];
        string slexeme = string(1, input[position]);
        position++;
        switch (lexeme) {
            case '+':
                return Token{ slexeme , PLUS};
            case '-':
                return Token{ slexeme , SUB };
            case '*':
                return Token{ slexeme , MUL };
            case '/':
                return Token{ slexeme , DIV };
            default:
                break;
        }
        
    }

    // 获取分隔符
    Token getSeparator() {
        char lexeme = input[position];
        string slexeme = string(1, input[position]);
        position++;
        switch (lexeme) {
        case '(':
            return Token{ slexeme , LEFT_BRACKET };
        case ')':
            return Token{ slexeme , RIGHT_BRACKET };
        case '[':
            return Token{ slexeme , LEFT_SQUARE_BRACKET };
        case ']':
            return Token{ slexeme , RIGHT_SQUARE_BRACKET };
        case '{':
            return Token{ slexeme , LEFT_CURLY_BRACKET };
        case '}':
            return Token{ slexeme , RIGHT_CURLY_BRACKET };
        case ';':
            return Token{ slexeme , SEMICOLON };
        case ',':
            return Token{ slexeme , COMMA };
        case '.':
            return Token{ slexeme , DOT };
        default:
            break;
        }
        return Token{ slexeme,RESERVED_WORD };
    }

};

//分析每行
void analyzeLine(const string& line , int no) {
    Lexer lexer(line);
    Token token;

    //cout <<"\033[32;1m第"<< no <<"行: \033[0m"<< line << endl;
    do {
        token = lexer.getNextToken();
        
        cout << "( " << token.lexeme << " , " << token.type << " )" << getType_Chinese(token.type,no)<<endl;
    } while (lexer.getPosition() != line.size());
}
//判断是否为字符


int main() {
    
    
    string filename="test.txt";
    string line;
    int n = 1;//行数

    //getline(cin, filename);

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "\033[32;1m打开文件错误\033[0m" << endl;
        return 1;
    }else cout << "\033[32;1m已打开文件:" << filename << "\033[0m" << endl;
    
    
    while (getline(inputFile, line)) {
        cout << "\033[1m\033[33;1m第"<< n << "行:" << line << "\033[0m" << endl;
        if (line.size() == 0) {
            n++;
            continue;
        }
        analyzeLine(line,n);
        n++;
    }

    inputFile.close();
    cout << "\033[32;1m文件已关闭\033[0m" << endl;
    
    //string a = "    string filename";
    //analyzeLine(a, 1);
}
//在最后一位为空格时会多出一次判断循环