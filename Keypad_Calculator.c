#include <reg51.h>
#include <string.h>

sbit rw = P1^1;
sbit rs = P1^2;
sbit en = P1^3;
sbit c1 = P2^0;
sbit c2 = P2^1;
sbit c3 = P2^2;
sbit c4 = P2^3;
sbit r1 = P2^4;
sbit r2 = P2^5;
sbit r3 = P2^6;
sbit r4 = P2^7;

void lcd();
void lcd_data(unsigned char a);
void lcd_command(unsigned char b);
void string(unsigned char *s);
void delay();
char get_key();
void display_result(int result);
void int_to_str(int num, char *str);

void main() {
    char key;
    int num1 = 0, num2 = 0, result = 0;
    char operator = 0;
    int flag = 0;

    lcd();  // Initialize the LCD

    while (1) {
        key = get_key();
        if (key != 0) {
            lcd_data(key);
            delay();
            if (key >= '0' && key <= '9') {
                if (flag == 0) {
                    num1 = num1 * 10 + (key - '0');
                } else {
                    num2 = num2 * 10 + (key - '0');
                }
            } else {
                if (key == '+' || key == '-' || key == '*' || key == '/') {
                    operator = key;
                    flag = 1;
                } else if (key == '=') {
                    switch (operator) {
                        case '+': result = num1 + num2; break;
                        case '-': result = num1 - num2; break;
                        case '*': result = num1 * num2; break;
                        case '/': result = (num2 != 0) ? (num1 / num2) : 0; break;
                    }
                    display_result(result);
                    num1 = num2 = result = flag = 0;
                    operator = 0;
                }
            }
        }
    }
}

char get_key() {
    // Check each column for a key press and return the corresponding character
    c1 = 0; c2 = 1; c3 = 1; c4 = 1;
    if (r1 == 0) { delay(); return '1'; }
    if (r2 == 0) { delay(); return '2'; }
    if (r3 == 0) { delay(); return '3'; }
    if (r4 == 0) { delay(); return '4'; }

    c1 = 1; c2 = 0; c3 = 1; c4 = 1;
    if (r1 == 0) { delay(); return '5'; }
    if (r2 == 0) { delay(); return '6'; }
    if (r3 == 0) { delay(); return '7'; }
    if (r4 == 0) { delay(); return '8'; }

    c1 = 1; c2 = 1; c3 = 0; c4 = 1;
    if (r1 == 0) { delay(); return '9'; }
    if (r2 == 0) { delay(); return '='; }
    if (r3 == 0) { delay(); return '@'; }
    if (r4 == 0) { delay(); return '&'; }

    c1 = 1; c2 = 1; c3 = 1; c4 = 0;
    if (r1 == 0) { delay(); return '*'; }
    if (r2 == 0) { delay(); return '/'; }
    if (r3 == 0) { delay(); return '-'; }
    if (r4 == 0) { delay(); return '+'; }

    return 0;  // No key pressed
}

void display_result(int result) {
    char buffer[16];
    int_to_str(result, buffer);
    lcd_command(0x01);  // Clear display
    string(buffer);
}

void int_to_str(int num, char *str) {
    int i = 0;
    int isNegative = 0;
    int start, end;
    char temp;

    // Handle 0 explicitly, otherwise empty string is printed for 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // If the number is negative, record that and make it positive
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    // If the number is negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate string

    // Reverse the string
    start = 0;
    end = i - 1;
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void lcd_command(unsigned char b) {
    P3 = b;
    rs = 0;
    rw = 0;
    en = 1;
    delay();
    en = 0;
}

void lcd_data(unsigned char a) {
    P3 = a;
    rs = 1;
    rw = 0;
    en = 1;
    delay();
    en = 0;
}

void lcd() {
    lcd_command(0x38);  // 2 lines and 5x7 matrix
    lcd_command(0x0c);  // display on, cursor off
    lcd_command(0x80);  // force cursor to beginning of 1st line
}

void string(unsigned char *s) {
    while (*s) {
        lcd_data(*s++);
    }
}

void delay() {
    int i;
    for (i = 0; i <20000; i++);
}
