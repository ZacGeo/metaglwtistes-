const int base = 2 + 3;
const float scale = base * 1.5;
const string greeting = "hello";
const int numbers[] = {1, 2, 3};

enum Color { RED, GREEN = 5, BLUE };

int matrix[][2] = {{1, 2}, {3, 4}};

int main() {
    int value;
    value = Color.BLUE + base;
    cout << greeting << scale << matrix[1][0] << value << Color.RED;
    return 0;
}
