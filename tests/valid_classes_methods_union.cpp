class Base {
    private:
        int secret;
    protected:
        int shared;
    public:
        int visible;
        int get(int);
};

class Derived : Base {
    public:
        int own;
        union {
            int integer_value;
            float float_value;
        };
        int sum(int);
};

union Data {
    int integer_value;
    float float_value;
};

int Base::get(int value) {
    return this.secret + value;
}

int Derived::sum(int value) {
    return shared + this.own + value;
}

int main() {
    Derived object;
    Base parent;
    Data data;
    object.visible = 1;
    object.own = 2;
    object.integer_value = 3;
    parent = object;
    data.integer_value = object.sum(4);
    cout << object.get(data.integer_value);
    return 0;
}
