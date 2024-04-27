#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Token 类型枚举
enum TokenType {
    RESERVED_WORD = 0,//识别之外
    IDENTIFIER,//标识符
    CONSTANT,//常数
    OPERATOR,//运算符
    SEPARATOR,//分隔符
    KEY_WORD//关键字
};

string getType(int i) {
    switch (i)
    {
    case 0:
        return "\033[31;1m无法识别的字符\033[0m";
    case 1:
        return "标识符";
    case 2:
        return "常数";
    case 3:
        return "运算符";
    case 4:
        return "分隔符";
    case 5:
        return "关键字";
    default:
        break;
    }
}

// Token 结构体，用于表示词法单元
struct Token {
    string lexeme;
    TokenType type;
};

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

        if (isalpha(currentChar)) {//非自定义
            // 识别标识符
            return getIdentifier();
        }
        else if (isdigit(currentChar)) {//非自定义
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
    size_t position;
    string operators = "+-*/=";//操作符字符表
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

    // 检查是否为关键字
    bool isKeyWord(string s) {
        for (int i = 0; i < 5; i++) {
            if (keywords[i] == s)return true;
        }
        return false;
    }
    
    //检查是否为标识符(未完成)
    bool isSignword() {

    }
    ;
    // 获取标识符后进行再判断是否为关键字
    Token getIdentifier() {
        string lexeme = "";
        while (position < input.size() && (isalnum(input[position]) || input[position] == '_')) {
            lexeme += input[position];
            position++;
        }
        if (isKeyWord(lexeme)) { return Token{ lexeme,KEY_WORD }; }
        return Token{ lexeme, IDENTIFIER }; 
    }

    // 获取常数
    Token getConstant() {
        string lexeme = "";
        while (position < input.size() && isdigit(input[position])) {
            lexeme += input[position];
            position++;
        }
        return Token{ lexeme, CONSTANT };
    }

    // 获取运算符
    Token getOperator() {
        string lexeme = string(1, input[position]);
        position++;
        return Token{ lexeme, OPERATOR };
    }

    // 获取分隔符
    Token getSeparator() {
        string lexeme = string(1, input[position]);
        position++;
        return Token{ lexeme, SEPARATOR };
    }

};

void analyzeLine(const string& line , int no) {
    Lexer lexer(line);
    Token token;

    cout <<"\033[32;1m第"<< no <<"行: \033[0m"<< line << endl;
    do {
        token = lexer.getNextToken();
        cout << "( " << token.lexeme << " , " << getType(token.type) << " )" << endl;
    } while (lexer.getPosition() != line.size());
}

int main() {
    string filename="test.txt";
    //getline(cin, filename);

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "打开文件错误" << endl;
        return 1;
    }
    string line;
    int n = 1;
    while (getline(inputFile, line)) {
        analyzeLine(line,n);
        n++;
    }
    inputFile.close();
}
