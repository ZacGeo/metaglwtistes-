int list values = [10, 20, 30, 40];

int main() {
    int first;
    int second;
    int count;
    int list tail;
    first = CAR(values);
    second = CADR(values);
    tail = CDR(values);
    tail = CDDR(values);
    count = length(values);
    cout << first << second << count;
    return 0;
}
