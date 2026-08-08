const int limit = 10;

int twice(int value) {
    int result;
    result = value * 2;
    return result;
}

int main() {
    int i;
    float total;
    i = 0;
    total = 0.0;
    while (i < limit) {
        total = total + twice(i);
        i++;
    }
    cout << total;
    return 0;
}
