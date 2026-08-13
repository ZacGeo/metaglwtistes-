class Base {
    private:
        int hidden;
};

class Derived : Base {
    public:
        int reveal();
};

int Derived::reveal() {
    return hidden;
}

int main() {
    return 0;
}
