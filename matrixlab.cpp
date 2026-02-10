#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <string>
#include <cctype>
#include "matrix.hpp"

struct Vars {
    std::string name;
    Matrix M;
    int order; // кол-во закрывающих скобок
};

void parser(const std::string& input){
    int balance = 0;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            ++balance;
        }
        if (input[i] == ')') {
            --balance;
        }
    }
    if (balance != 0) {
        throw std::invalid_argument("not currect order balance");
    }
}

void Executor(std::stringstream& out, std::stack<Vars>& S, std::vector<Vars>& buffer, const std::string& cmd_name) {
    if (cmd_name == "=") {
                        // взять переменную из стека и у нее имя, взять следущую переменную из стека и ее значение и записать это в буффер
                        std::string name_tmp = S.top().name;
                        S.pop();
                        Matrix value_tmp = S.top().M;
                        S.pop();
                        Vars result = {name_tmp, value_tmp, 0};
                        buffer.push_back(result);
                    } else if (cmd_name == "+") {
                        // сложить две следущие переменные и минусануть скобки у последней создать новую и вернуть в стек
                        Matrix tmp_1 = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        Matrix value = tmp_1 + tmp_2;
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "-") {
                        // сразу 2 варианты (унарный минус либо же бинарный, надо искать конечную скобочку)
                        // достаем переменную, если ее скобки равны нулю, берем следущую и минусуем их,
                        // иначе выполняем бинарный минус и минусуем скобку у переменной
                        bool unar = (S.top().order != 0);
                        if (unar) {
                            Matrix value = -S.top().M;
                            int order_tmp = S.top().order - 1;
                            S.pop();
                            Vars result = {"", value, order_tmp};
                            S.push(result);
                        } else {
                            Matrix tmp_1 = S.top().M;
                            S.pop();
                            Matrix tmp_2 = S.top().M;
                            Matrix value = tmp_1 - tmp_2;
                            int order_tmp = S.top().order - 1;
                            S.pop();
                            Vars result = {"", value, order_tmp};
                            S.push(result);
                        }
                    } else if (cmd_name == ".*") {
                        Matrix value = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        value.elemental_multiplication(tmp_2);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "./") {
                        Matrix value = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        value.elemental_division(tmp_2);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "*") {
                        // умножаем матрицы и минусуем скобку => кладем в стек
                        Matrix tmp_1 = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        Matrix value = tmp_1 * tmp_2;
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "/") {
                        Matrix tmp_1 = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        Matrix value = tmp_1 / tmp_2;
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "zeros") {
                        Rational n = S.top().M.to_rational();
                        S.pop();
                        Rational m = S.top().M.to_rational();
                        int order_tmp = S.top().order -1;
                        S.pop();
                        Matrix value(static_cast<size_t>(n.to_int()), static_cast<size_t>(m.to_int()), Rational(0));
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "ones") {
                        Rational n = S.top().M.to_rational();
                        S.pop();
                        Rational m = S.top().M.to_rational();
                        int order_tmp = S.top().order -1;
                        S.pop();
                        Matrix value(static_cast<size_t>(n.to_int()), static_cast<size_t>(m.to_int()), Rational(1));
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "eye") {
                        Rational n = S.top().M.to_rational();
                        int order_tmp = S.top().order -1;
                        S.pop();
                        Matrix value = Matrix::unit(static_cast<size_t>(n.to_int()));
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "linspace") {
                        Rational x_0 = S.top().M.to_rational();
                        S.pop();
                        Rational x_1 = S.top().M.to_rational();
                        S.pop();
                        Rational n = S.top().M.to_rational();
                        int order_tmp = S.top().order -1;
                        Matrix value = Matrix::linspace(x_0, x_1, n);
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "vertcat") {
                        Matrix value = S.top().M;
                        S.pop();
                        while (S.top().order == 0) {
                            value.vertcat(S.top().M);
                            S.pop();
                        }
                        value.vertcat(S.top().M);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "horzcat") {
                        Matrix value = S.top().M;
                        S.pop();
                        while (S.top().order == 0) {
                            value.horzcat(S.top().M);
                            S.pop();
                        }
                        value.horzcat(S.top().M);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "transpose") {
                        Matrix value = S.top().M.transpose();
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "det") {
                        Rational det = S.top().M.determinant();
                        Matrix value(1,1,det);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "inv") {
                        Matrix value = S.top().M.inverse();
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "sum") {
                        Rational det = S.top().M.sum();
                        Matrix value(1,1,det);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "prod") {
                        Rational det = S.top().M.product();
                        Matrix value(1,1,det);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "min") {
                        Matrix tmp_1 = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        Matrix value = min(tmp_1, tmp_2);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "max") {
                        Matrix tmp_1 = S.top().M;
                        S.pop();
                        Matrix tmp_2 = S.top().M;
                        Matrix value = max(tmp_1, tmp_2);
                        int order_tmp = S.top().order - 1;
                        S.pop();
                        Vars result = {"", value, order_tmp};
                        S.push(result);
                    } else if (cmd_name == "disp") {
                        if (!S.top().name.empty()) {
                            out << S.top().name << " = " << S.top().M.to_string() << std::endl;
                        } else {
                            out << "ans = " << S.top().M.to_string() << std::endl;
                        }
                    } else {
                        throw std::invalid_argument("ERROR");
                    }
}

int main(int argc, char* argv[]) {

    std::ifstream in_file(argv[1], std::ios_base::binary);
    if (!in_file) {
        std::cerr << "Can not open file: " << argv[1];
        return 1;
    }

    std::ofstream out_file(argv[2], std::ios_base::binary);
    if(!out_file) {
        std::cerr << "Can not open file: " << argv[2];
        in_file.close();
        return 1;
    }
    int number_of_line = 0;
    std::stringstream out_stream;
    std::string input;
    std::vector<Vars> buffer;
    while(std::getline(in_file, input)) {
        try{
            ++number_of_line;
            input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
            parser(input);
            std::stack<Vars> S;
            std::string word;
            for (int i = static_cast<int>(input.size()) - 1; i >= 0; --i) {
                if (input[i] == ' ') {
                    std::reverse(word.begin(), word.end());
                    if (word[0] == '(') {
                        std::string cmd_name = word.substr(1);
                        // написать все созможные команды и соответсвующие обработки для стека
                        Executor(out_stream, S,buffer,cmd_name); 
                    } else {
                        size_t iterator = 0;
                        size_t size = word.size();
                    
                        while (word[iterator] != ')' && size > iterator) {
                           ++iterator;
                        }
                        std::string var_name = word.substr(0,iterator);
                        int order = static_cast<int>(size - iterator); // кол-во закрывающих скобок
                    
                        // рассмотрим случай для ввода ТОЛЬКО целых чисел, для рациональных алалогично
                        bool flag = true;
                        for (int i = 0; i < var_name.size(); ++i) {
                            if (isdigit(var_name[i]) == 0) {
                                flag = false;
                            }
                        }
                        if (flag) {
                            Rational val = Rational(BigInteger(var_name));
                            Vars tmp = {"", Matrix(1,1,val), order};
                            S.push(tmp);
                        }
                        else {
                            // либо берем из буффера, либо сохраняем только имя для функций типа: =
                            bool find_val = false;
                            for (int i = 0; i < buffer.size(); ++i) {
                                if (buffer[i].name == var_name) {
                                    find_val = true;
                                    Vars tmp = {var_name, buffer[i].M, order};
                                    S.push(tmp);
                                    break;
                                }
                            }
                            if (!find_val) {
                                // добавляем только имя и фиктивные переменные
                                Vars tmp = {var_name, Matrix(1,1,Rational(0)), order};
                                S.push(tmp);
                            }
                        }
                    }
                word = "";
                continue;
                }
            word += input[i];
            }
            if (!word.empty()) {
                // догбавить код для последнего слова, нужно просто скопировать сверхну в обработке накопленного слова
                std::reverse(word.begin(), word.end());
                std::string cmd_name = word.substr(1);
                Executor(out_stream, S,buffer,cmd_name);      
            }
        } catch (...) {
            out_file << "Error in line " << number_of_line;
            return 0;
        }  
    } 
    out_file << out_stream.rdbuf();
    return 0;
}





    