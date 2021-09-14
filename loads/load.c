char x[100];

void test() {
    for (int i = 0; i < 100; i++) {
        if (i > 30) {
            x[i] += i;
        }
    }
}

int main(void) {
    int sum;
    for (int i = 0; i < 100; i++) {
        sum += x[i];
    }
    return sum;
}
