class Mixed {
    public:
        char tag;
        int value;
};

Mixed global_item = {'G', 7};

int main() {
    Mixed local_item;
    float number;
    int before;
    int truth;
    local_item.tag = 'L';
    local_item.value = 9;
    number = 2.0;
    before = number++;
    truth = number && before;
    cout << global_item.tag << local_item.value << number << before << truth;
    return 0;
}
