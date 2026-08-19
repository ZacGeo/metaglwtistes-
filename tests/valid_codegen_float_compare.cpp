int main() {
    float left;
    float right;
    int result;
    left = 1.25;
    right = 2.5;
    if (left < right) {
        result = 1;
    } else {
        result = 0;
    }
    if (right >= left) {
        result = result + 1;
    }
    cout << result;
    return 0;
}
