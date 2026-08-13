class Pair {
    public:
        int first;
        int second;
};

int flat[3][2] = {0, 1, 4, -2, 0, -3};
Pair pairs[] = {{0, 2}, {1, -3}, {-4, 0}};

int main() {
    cout << flat[2][1] << pairs[1].second;
    return 0;
}
