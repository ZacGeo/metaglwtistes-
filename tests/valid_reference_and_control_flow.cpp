int increment(int&);

int increment(int &value) {
    value++;
    return value;
}

int choose(int value) {
    if (value > 0)
        return value;
    else
        return -value;
}

int main() {
    int number;
    number = 2;
    while (number < 4) {
        increment(number);
        if (number == 3)
            continue;
    }
    cout << choose(number);
    return 0;
}
